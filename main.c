#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <includes/codexion.h>

char ARG_ERR_MESSAGE[] = "Insufficient number of aruguments\n";
char PARSE_ERR_MESSAGE[] = "Invalid arguments included\n";

int main(int argc, char** argv) 
{
    if (argc != 8) {
        write(1, ARG_ERR_MESSAGE, strlen(ARG_ERR_MESSAGE));
    }
    t_params params;
    if (parse(argc, argv, &params) != 0)
    {
        write(1, PARSE_ERR_MESSAGE, strlen(PARSE_ERR_MESSAGE));
    }

    
    return 0;
}