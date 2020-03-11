#define __OWA3X_AN3_CPP

// System internal includes
#include <iostream>
#include <fstream> 
using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>


// OWA libraries to include
#include <owa4x/pm_messages.h>
#include <owa4x/owerrors.h>

//User libs 
#include "owa4x_io_adapter.h"
#include "LoadLibs.h"

void unload_libraries_on_error(void *lib_io_handle, void *lib_rtu_control_handle);
bool check_file_exists(char const *path_to_file);
char* concat_char_ptrs(char const *left_char_ptr, char const *right_char_ptr);
int check_pin_direction(int pin_number);
int write_value_input_pin(int pin_number, bool value);
int read_value_output_pin(int pin_number);
int write_value_analog_input(int input_number, int value);
static int get_dig_in(int pin_number);
static int set_dig_out(int pin_number, int value);
static int get_ana_in(int pin_number);

int main (void) {
    void  *lib_io_handle = NULL;
    void  *lib_rtu_control_handle = NULL;
    const char base_path[] = "/sys/class/gpio/gpio";
    char *gpio_dir;
    int pause = 0;
    ofstream hwmons[4];

    // Starting both libraries
    LoadExternalLibrary((char *)LIBRTU, &lib_rtu_control_handle);
    LoadExternalLibrary((char *)LIBIO, &lib_io_handle);

    LoadRTUControlFunctions(lib_rtu_control_handle);
    LoadIOSFunctions(lib_io_handle);

    if ((*FncRTUControl_Initialize)(NULL) != NO_ERROR) {
       unload_libraries_on_error(lib_io_handle, lib_rtu_control_handle);
    }
    if ((*FncIO_Initialize)() != NO_ERROR) {
       unload_libraries_on_error(lib_io_handle, lib_rtu_control_handle);
    }

    if ((*FncRTUControl_Start)() != NO_ERROR) {
       unload_libraries_on_error(lib_io_handle, lib_rtu_control_handle);
    }
    if ((*FncIO_Start)() != NO_ERROR) {
       unload_libraries_on_error(lib_io_handle, lib_rtu_control_handle);
    }

    // Check if config file exists
    if(check_file_exists("/opt/owa_adapter_config.txt")) {
        char file_path[] = "/opt/owa_adapter_config.txt";
        char file_content[128];
        char *temp_read;
        ifstream config_file;

        config_file.open(file_path);
        config_file.getline(file_content, 128);
        config_file.close();

        temp_read = strtok(file_content, "=");
        if(temp_read != NULL) {
            temp_read = strtok(NULL, "=");
            if(temp_read != NULL) {
                pause = atoi(temp_read);
            }
        }
    }

    // Create files to the ADC
    if(!check_file_exists("/opt/hwmon/hwmonOWA450")) {
        system("mkdir -p /opt/hwmon/hwmonOWA450");
        char char_i[10];
        char path[100];
        for(int i = 0; i < 4; i++) {
            sprintf(char_i, "%d", i+1);
            strcpy(path, "/opt/hwmon/hwmonOWA450/ain-");
            strcat(path, char_i);
            hwmons[i].open(path);
            hwmons[i] << 0;
            hwmons[i].close();
        }
    }

    char pin_char[10];
    for(int i = 0; i < 10; i++) {
        sprintf(pin_char, "%d", i);
        gpio_dir = concat_char_ptrs(&base_path[0], &pin_char[0]);
        if (check_file_exists(gpio_dir)) {
            int value;
            switch(check_pin_direction(i)) {
                case 0:
                    value = get_dig_in(i);
                    write_value_input_pin(i, value);
                    break;
                case 1:
                    value = read_value_output_pin(i);
                    set_dig_out(i, value);
                    break;
            }
        }
        free(gpio_dir);
    }

    for(int i = 1; i <= 4; i++) {
        int value = get_ana_in(i-1);
        write_value_analog_input(i, value);
    }

    if (pause > 0) {
        sleep(pause);
    }

    return 0;
}

/**
 * Function:    unload_libraries_on_error()
 * Input:       
 *              void *lib_io_handle: Pointer to I/O library.
 *              void *lib_rtu_control_handle: Pointer to RTU library.
 * Description: This function get the value of the specified input pin
 */
void unload_libraries_on_error(void *lib_io_handle, void *lib_rtu_control_handle) {
    UnloadExternalLibrary(lib_rtu_control_handle);
    UnloadExternalLibrary(lib_io_handle);
}

/**
 * Function:    get_dig_in()
 * Input:       
 *              int pin_number: Number of the pin from which this method will get the value
 * Output:      
 *              Otherwise   - Value getted from the pin
 *              -1          - ERROR: Couldn't get any value to the specified pin
 * Description: This function get the value of the specified input pin
 */
static int get_dig_in(int pin_number) {
    int ret_value;
    unsigned char read_value;
    ret_value = (*FncDIGIO_Get_DIN)((unsigned char)pin_number, &read_value);
    if(ret_value == 0) {
        return (int)read_value;
    }
    return -1;
}

/**
 * Function:    set_dig_out()
 * Input:       
 *              int pin_number: Number of the pin from which this method will set the value
 *              int value: Value (0/1) that the function have to set on the output pin
 * Output:      
 *              Otherwise   - Value setted to the pin
 *              -1          - ERROR: Couldn't set any value to the specified pin
 * Description: This function set a value in a specified output pin
 */
static int set_dig_out(int pin_number, int value) {
    int wrote_val = (*FncDIGIO_Set_DOUT)((unsigned char)pin_number, (unsigned char)value);
    if(wrote_val == 0) {
        return value;
    }
    return -1;
}

/**
 * Function:    get_ana_in()
 * Input:       
 *              int pin_number: Number of the pin from which this method will get the value
 * Output:      
 *              Otherwise   - Value of the pin
 *              -1          - ERROR: Couldn't get any value of the specified pin
 * Description: This function get the value of an analog input
 */
static int get_ana_in(int pin_number) {
    int value;
    int ret_value = (*FncANAGIO_GetAnalogIn)((unsigned char)pin_number, &value);
    if(ret_value == 0) {
        return value;
    }
    return -1;
}

/**
 * Function:    check_file_exists()
 * Input:       
 *              char const *right_char_ptr: Path to the file we want to check if exists
 * Output:      
 *              true  - File specified exists
 *              false - File specified not found
 * Description: This function check if a file specified by parameters exists or not
 */
bool check_file_exists(char const *path_to_file) {
    struct stat info;

    if(stat(path_to_file, &info) == 0) {
        return true;
    }
    return false;
}

/**
 * Function:    concat_char_ptrs()
 * Input:       
 *              char const *left_char_ptr: Char array pointer to the first part of array we want to concatenate
 *              char const *right_char_ptr: Char array pointer to the final part of array we want to concatenate
 * Output:      
 *              Concatenated char array. WARNING: This use malloc, remember to free this space of memory
 * Description: This function will concatenate two char array pointer in one, returning the concatenated result.
 */
char* concat_char_ptrs(char const *left_char_ptr, char const *right_char_ptr) {
    char *file_path = (char*)malloc(sizeof(char)*200);

    strcpy(file_path, left_char_ptr);
    strcat(file_path, right_char_ptr);

    return file_path;
}

/**
 * Function:    check_pin_direction()
 * Input:       
 *              int input_number: Number of the pin we will check
 * Output:      
 *              0   - In case of the pin has direction INPUT
 *              1   - In case of the pin has direction OUTPUT
 *              -1  - Otherwise (ERROR: Not expected direction value)
 * Description: This function will check if the pin is an INPUT or OUTPUT pin, we expect that the pin is exported,
 *              if isn't exported or direction is a not expected value, this will return -1 as generic error code.
 */
int check_pin_direction(int pin_number) {
    char pin_char[10], base_path[] = "/sys/class/gpio/gpio", direction_file_relative_path[] = "/direction";
    char *temp_path;
    char *direction_file_path;
    char file_content[128];
    ifstream direction_file;

    sprintf(pin_char, "%d", pin_number);
    temp_path = concat_char_ptrs(&base_path[0], &pin_char[0]);
    if(!check_file_exists(temp_path)) {
        free(temp_path);
        return -1;
    }
    
    direction_file_path = concat_char_ptrs(&temp_path[0], &direction_file_relative_path[0]);

    direction_file.open(direction_file_path);
    direction_file.getline(file_content, 128);
    direction_file.close();

    free(temp_path);
    free(direction_file_path);

    if(strncmp(file_content, "in", strlen(file_content)) == 0) {
        return 0;
    }
    if(strncmp(file_content, "out", strlen(file_content)) == 0) {
        return 1;
    }

    return -1;
}

/**
 * Function:    write_value_input_pin()
 * Input:       
 *              int input_number: Number of the input we will write.
 *              bool value: Value we will write on the on the input.
 * Output:      
 *              -1          - Error on write.
 *              Otherwise   - Value write from file.
 * Description: Write the value specified by parameter onto the file of input with number specified by parameter.
 */
int write_value_input_pin(int pin_number, bool value) {
    char pin_char[10], base_path[] = "/sys/class/gpio/gpio", active_low_file_relative_path[] = "/active_low";
    char *active_low_file_path;
    ofstream active_low_file;

    sprintf(pin_char, "%d", pin_number);
    active_low_file_path = concat_char_ptrs(&base_path[0], &pin_char[0]);
    if(!check_file_exists(active_low_file_path)) {
        free(active_low_file_path);
        return -1;
    }

    int i = strlen(active_low_file_path);
    for(unsigned int j = 0; j < strlen(active_low_file_relative_path); i++, j++) {
        active_low_file_path[i] = active_low_file_relative_path[j];
    }
    active_low_file_path[i] = '\0';

    active_low_file.open(active_low_file_path);
    active_low_file << value;
    active_low_file.close();

    free(active_low_file_path);
    
    return value;
}

/**
 * Function:    read_value_output_pin()
 * Input:       
 *              int input_number: Number of the output we will write
 * Output:      
 *              -1          - Error on read
 *              Otherwise   - Value readed from file
 * Description: Read value from the file of output with number specified by parameter
 */
int read_value_output_pin(int pin_number) {
    char pin_char[10], base_path[] = "/sys/class/gpio/gpio", value_file_relative_path[] = "/value";
    char *value_file_path;
    char file_content[128];
    ifstream value_file;

    sprintf(pin_char, "%d", pin_number);
    value_file_path = concat_char_ptrs(&base_path[0], &pin_char[0]);
    if(!check_file_exists(value_file_path)) {
        free(value_file_path);
        return -1;
    }

    int i = strlen(value_file_path);
    
    for(unsigned int j = 0; j < strlen(value_file_relative_path); i++, j++) {
        value_file_path[i] = value_file_relative_path[j];
    }
    value_file_path[i] = '\0';


    value_file.open(value_file_path);
    value_file.getline(file_content, 128);
    value_file.close();

    free(value_file_path);

    if(strncmp(file_content, "0", strlen(file_content)) == 0) {
        return 0;
    }
    if(strncmp(file_content, "1", strlen(file_content)) == 0) {
        return 1;
    }

    return -1;
}

/**
 * Function:    write_value_analog_input()
 * Input:       
 *              int input_number: Number of the input we will write
 *              int value: Value we will write on the on the input
 * Output:      
 *              -1          - Error on write
 *              Otherwise   - Value wrote on file
 * Description: Write value specified by parameter onto the file of input with number specified by parameter
 */
int write_value_analog_input(int input_number, int value) {
    char input_char[10], base_path[] = "/opt/hwmon/hwmonOWA450/ain-";
    char *analog_value_file_path;
    ofstream analog_value_file;

    sprintf(input_char, "%d", input_number);
    analog_value_file_path = concat_char_ptrs(&base_path[0], &input_char[0]);
    if(!check_file_exists(analog_value_file_path)) {
        free(analog_value_file_path);
        return -1;
    }
    analog_value_file.open(analog_value_file_path);
    analog_value_file << value;
    analog_value_file.close();

    free(analog_value_file_path);
    
    return value;
}