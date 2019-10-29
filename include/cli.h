#define     LINE_BUF_SIZE       128
#define     ARG_BUF_SIZE        64
#define     MAX_NUM_ARGS        8

String readerMode = "";
String nim = "";
String name = "";

int ledPin = LED_BUILTIN;
bool error_flag = false;

char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];

//Function declarations
int cmd_help();
int cmd_exit();
int cmd_read();
int cmd_write();
 
//List of functions pointers corresponding to each command
int (*commands_func[])(){
    &cmd_help,
    &cmd_read,
    &cmd_write,
    &cmd_exit
};
 
//List of command names
const char *commands_str[] = {
    "help",
    "read",
    "write",
    "exit"
};

int num_commands = sizeof(commands_str) / sizeof(char *);

void cli_init(){
    Serial.println("Welcome to this simple Arduino command line interface (CLI).");
    Serial.println("Type \"help\" to see a list of commands.");
}

void read_line(){
    String line_string;
 
    while(!Serial.available());
 
    if(Serial.available()){
        line_string = Serial.readStringUntil(';');
    }
    
    if(line_string.length() < LINE_BUF_SIZE){
        line_string.toCharArray(line, LINE_BUF_SIZE);
        Serial.print(line_string);
    }
    else{
        Serial.println("Input string too long.");
        error_flag = true;
    }
}
 
void parse_line(){
    char *argument;
    int counter = 0;
 
    argument = strtok(line, " ");
 
    while((argument != NULL)){
        if(counter < MAX_NUM_ARGS){
            if(strlen(argument) < ARG_BUF_SIZE){
                strcpy(args[counter],argument);
                argument = strtok(NULL, " ");
                counter++;
            }
            else{
                Serial.println("Input string too long.");
                error_flag = true;
                break;
            }
        }
        else{
            break;
        }
    }
}

int execute(){  
    for(int i=0; i<num_commands; i++){
        if(strcmp(args[0], commands_str[i]) == 0){
            return(*commands_func[i])();
        }
    }
 
    Serial.println("Invalid command. Type \"help\" for more.");
    return 0;
}

void my_cli(){
    Serial.print("> ");
 
    read_line();
    if(!error_flag){
        parse_line();
    }
    if(!error_flag){
        execute();
    }
 
    memset(line, 0, LINE_BUF_SIZE);
    memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);
 
    error_flag = false;
}
 
void help_help(){
    Serial.println("The following commands are available:");
 
    for(int i=0; i<num_commands; i++){
        Serial.print("  ");
        Serial.println(commands_str[i]);
    }
    Serial.println("");
    Serial.println("You can for instance type \"help read\" for more info on the LED command.");
}

void help_exit(){
    Serial.println("This will exit the CLI. To restart the CLI, restart the program.");
}

void help_read() {
    Serial.println("Read data from RFID if exist");
}

void help_write() {
    Serial.println("Write data to RFID on first block");
    Serial.println("Usage Sample: ");
    Serial.println(" write 1234567890");
    Serial.println("This will write '1234567890' on first block of RFID Tag");
}

int cmd_help(){
    if(args[1] == NULL){
        help_help();
    }
    else if(strcmp(args[1], commands_str[0]) == 0){
        help_help();
    }
    else if(strcmp(args[1], commands_str[1]) == 0){
        help_read();
    }
    else if(strcmp(args[1], commands_str[2]) == 0){
        help_write();
    }
    else if(strcmp(args[1], commands_str[3]) == 0){
        help_exit();
    }
    else{
        help_help();
    }
}

int cmd_read(){
    Serial.println("Waiting RFID..");
    readerMode = "read";
}

int cmd_write(){
    Serial.println("Writing data to RFID...");
    readerMode = "write";
}
 
int cmd_exit(){
    Serial.println("Exiting CLI.");
    ESP.restart();
}