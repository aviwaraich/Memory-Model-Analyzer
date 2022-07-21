#include <stdio.h>   //Main Functions
#include <stdbool.h>  //Bools
#include <string.h>  //String Functions
#include <stdlib.h>  //Malloc Functions

typedef struct datacollect
{
    /*
     Data Set to Save Information regarding the code in full lines form
     */
    char *exec;  //Name of File
    char *ROData;  //Ro Data
    char *static_data; //Static Data
    char *heap; //Heap Data
    char *stack; //Stack Data
    char *RODatafunctions; //Scope of RO Data
    char *stackfunctions; //Scope of Stack Data
    char *heapfunctions; //Scope of Heap Data
    char *static_datafunctions; //Scope of Static Data
    int linescount; //Total Line Count
    char *numberoffunction; //Name of Functions
    int linesperfumctiom; //Number of Functions
    int variablesperfunction; //Variable Per Function
} datacollect;

char *MallocString(char *string)
{
    /*
     Allocates Memory for the string and retuns the copy of the given string
     */
    char *tempstring = (char*)malloc(strlen(string)+1);
    strcpy(tempstring, string); //Copy New String
    return tempstring;
}

int commacounter(char *line)
{
    /*
     Counts the number of comma the line has and returns int
     */
    int count=0,comma=0;
    while(count<strlen(line))
    {
        if(line[count]==',') //if it is comma
        {
            comma = comma + 1; //Counts to commaa counter
        }
        count = count + 1;
    }
    return comma;
}

bool function_checker(char *string, char *nextstring)
{
    /*
     Functions Input a string and next line next to the string and find if this is a line which initiates a function or not.
     */
    char *Function_Names[4] = {"int","float","char","void"};
    for (int i=0; i<4; i++)
    {
        if (strstr(string,Function_Names[i])!=NULL && strstr(string,"(")!=NULL && strstr(string,")")!=NULL && ((strstr(string,"{")!=NULL) || (strstr(nextstring,"{")!=NULL)))
        {
            return true; //Retursn True if it contains all of the if statemnts and tell it is function
        }
    }
    return false;
}

bool ro_checker(char *line)
{
    /*
     Functions Input a string and find if this is a line which is a string RO - Read only pointer or not.
     */
    if(((strncmp(" char*",line,6)==0)||(strncmp("char*",line,5)==0)) && strstr(line,"=\"")!=NULL)
    {
        return true; //Retursn True if it contains all of the if statemnts and tell it is RO Data
    }
    return false;
}

void space_remover(char *line)
{
    /*
     Functions remove all the empty space from a string from the given input through the functionality of pointers
     */
    int i = 0, j = 0;
    while (line[i])
    {
        if (line[i] != ' ')
            line[j++] = line[i]; //Removes Spaew in a string if it has ' ' in it
        i++;
    }
    line[j] = '\0';
}

char alloc_checker(char *line)
{
    /*
     Functions Input a string and find if this is a line is Malloc/Calloc or not.
     */
    char *alloc[4] = {"=malloc","=calloc",")malloc",")calloc"};
    for (int i=0; i<4; i++)
    {
        if (strstr(line,alloc[i])!=NULL && strstr(line,"(")!=NULL && strstr(line,")")!=NULL &&  (strstr(line,";")!=NULL))
        {
            return true; //Retursn True if it contains all of the if statemnts and tell it is Heap Data
        }
    }
    return false;
}

void printeOData(int ROData_count, datacollect array[])
{
    /*
     Functions prints the Ro Data
     */
    printf("### ROData ###       scope  type  size\n");
    for (int i=0; i<ROData_count; i=i+1)
    {
        char *temp = MallocString(array[i].ROData);
        char *name = strtok(temp,"*");
        name = strtok(NULL,"=");  //Name of RO Data
        
        temp = MallocString(array[i].ROData);
        char *size = strtok(temp,"\"");
        size = strtok(NULL,"\""); //Size of RO Data
        
        if (size==NULL)
            printf("     %s            %s  char[]   %lu\n",name,array[i].RODatafunctions,sizeof(char)); //If String "" is this
        else
            printf("     %s            %s  char[]   %lu\n",name,array[i].RODatafunctions,(strlen(size)+1)*sizeof(char)); //if String "hELLO" is this
    }
}

void printalloc(char*line,char*function)
{
    /*
     Functions prints the heap data with malloc/calloc
     */
    char *temp = MallocString(line);
    char *name = strtok(temp,"="); //Name of Alloc
    char *type = strtok(NULL,"("); //Name of Type
    char *size = strtok(NULL,""); //Size
    size[strlen(size)-1]='\0';
    if(strstr(name,"*")!=NULL) //if int*i=malloc
    {
        if(strstr(type,"malloc")!=NULL)
            printf("     %s            %s  malloc   %s\n",name,function,size); //Malloc
        else
            printf("     %s            %s  calloc   %s*%s\n",name,function,strtok(size,","),strtok(NULL,"")); //Calloc
    }
    else
    {
        if(strstr(type,"malloc")!=NULL) //if inti=malloc or i=malloc
            printf("     *%s            %s  malloc   %s\n",name,function,size); //Malloc
        else
            printf("     *%s            %s  calloc   %s*%s\n",name,function,strtok(size,","),strtok(NULL,"")); //Calloc
    }
}
long int sizeteller(int number)
{
    /*
     Function gets to know what number of the variable we have then outputs the size of the variable so other print statements can use it
     */
    if(number==0) //int*
        return sizeof(int*);
    else if(number==1) //float*
        return sizeof(float*);
    else if(number==2) //char*
        return sizeof(char*);
    else if(number==3) //int
        return sizeof(int);
    else if(number==4) //float
        return sizeof(float);
    else //char
        return sizeof(char);
}
void print_staticNstack(char*line,char*function)
{
    /*
     Functions prints the the static data and stack data in the struct line by line
     */
    char *Function_Names[6] = {"int*","float*","char*","int","float","char"};
    for (int j=0; j<6; j=j+1)
    {
        if(strncmp(Function_Names[j],line,strlen(Function_Names[j]))==0)
        {
            if(strstr(line,"]={")!=NULL && strstr(line,"[")!=NULL && strstr(line,"}")!=NULL && j!=2 && ((j!=5)||commacounter(line)>0)) //variable[]={} check
            {
                char *temp = MallocString(line);
                char *name = strtok(temp,"[");
                char *size = strtok(NULL,"]");
                if(strstr(size,"=")!=NULL) //int[]={4,5.6} check
                    printf("     %s            %s  %s[%d]   %d*sizeof(%s)\n",name+strlen(Function_Names[j]),function,Function_Names[j],commacounter(size)+1,commacounter(size)+1,Function_Names[j]);
                else //- int[5]={4,5.6}
                    printf("     %s            %s  %s[%s]   %s*sizeof(%s)\n",name+strlen(Function_Names[j]),function,Function_Names[j],size,size,Function_Names[j]);
            }
            else if(strstr(line,"[")!=NULL && line[strlen(line)-1]==']' && j!=2 && j!=5) //int[5]; check
            {
                char *temp = MallocString(line);
                char *name = strtok(temp,"[");
                char *size = strtok(NULL,"]");
                printf("     %s            %s  %s[%s]   %s*sizeof(%s)\n",name+strlen(Function_Names[j]),function,Function_Names[j],size,size,Function_Names[j]);
            }
            else if(j==5 && strstr(line,"[")!=NULL && strstr(line,"]")!=NULL) //char[5] or char[] check
            {
                char *temp = MallocString(line);
                char *name = strtok(temp,"[");
                char *size = strtok(NULL,"]");
                if(strstr(size,"=")!=NULL) //char[N]="hie"
                {
                    size = strtok(size,"\"");
                    size = strtok(NULL,"\"");
                    printf("     %s            %s  %s[%lu]   %lu*sizeof(%s)\n",name+strlen(Function_Names[j]),function,Function_Names[j],strlen(size)+1,strlen(size)+1,Function_Names[j]);
                }
                else
                {
                    printf("     %s            %s  %s[%s]   %s*sizeof(%s)\n",name+strlen(Function_Names[j]),function,Function_Names[j],(size),(size),Function_Names[j]);
                }
            }
            else //everything else such as inti; or inti=5
            {
                char *temp = MallocString(line);
                char *name = strtok(temp,"=");
                printf("     %s            %s  %s   %lu\n",name+strlen(Function_Names[j]),function,Function_Names[j],sizeteller(j));
            }
            break;
        }
    }
}


int main(int argc, char *argv[])
{
    FILE *file = fopen(argv[1],"r");
    if (file != NULL) //if file it not open
    {
        printf(">>> Memory Model Layout <<<\n\n"); //print Name
        printf("***  exec // text ***\n");
        printf("   %s\n\n",argv[1]);
        
        fseek (file, 0, SEEK_END);
        int size = ftell(file); //Find the size of the file
        rewind(file);

        int ROData_count = 0,static_data_count = 0,heap_count = 0,stack_count = 0,numberoffunction_count = 0, comma =0; //To count of the data types abalivlabe
        char line[size],ogline[size],nextline[size]; //save next line and this line
        char *token;
        bool function_check = false; //Line is not function from start
        char *Function_Names[7] = {"int*","float*","char*","int","float","char","void"};
        char *functionname; //Name of the function
        
        datacollect *array = malloc(size * sizeof(datacollect)); //Allocte space for the the struct
        
        array[0].exec=argv[1]; //Save the name of the file
        array[0].linescount = 0;
        
        while (fgets(line,sizeof(line),file)!=NULL)
        {
            array[0].linescount++; //Counting the Number of the line
            
            strcpy(ogline,line);
            space_remover(line);

            fpos_t position;
            fgetpos(file, &position);
            fgets(nextline,sizeof(nextline),file); //Code to save the next line in fgets
            fsetpos(file, &position);
            
            function_check = function_checker(line,nextline); //Check it is function
            
            array[stack_count].stackfunctions = MallocString("global"); //Everything starts as a global
            array[ROData_count].RODatafunctions = MallocString("global");
            array[heap_count].heapfunctions = MallocString("global");
            array[static_data_count].static_datafunctions = MallocString("global");
            
            if(function_check) //If it is a function
            {
                functionname = strtok(ogline,"(");
                functionname = strtok(ogline," ");
                functionname = strtok(NULL,""); //Get the name of the fuction
                if(strstr(functionname,"*")!=NULL)
                    functionname=functionname+1; //Get the name of the fuction add 1 if *
                
                array[stack_count].stackfunctions = MallocString(functionname); //Saves the function name to each data types
                array[ROData_count].RODatafunctions = MallocString(functionname);
                array[heap_count].heapfunctions = MallocString(functionname);
                array[static_data_count].static_datafunctions = MallocString(functionname);
                array[numberoffunction_count].numberoffunction = MallocString(functionname);
                
                while (ogline[0]!='}') //Keep going through the until the end of the line
                {
                    comma = commacounter(line); //get number of comma
                    if(function_check) //if line is a function
                    {
                        char *linecopy = MallocString(line);
                        char *tokencopy = strtok(linecopy,")");
                        
                        token = strtok(line,"(");
                        token = strtok(NULL, ")");
                        token = strtok(token, ",");
                        
                        if(token!=NULL && tokencopy[strlen(tokencopy)-1]!='(') //Get the variable declared inside the function
                        {
                            array[stack_count].stack = MallocString(token);
                            if(array[stack_count].stackfunctions == NULL)
                                array[stack_count].stackfunctions = MallocString(array[stack_count-1].stackfunctions);
                            array[numberoffunction_count].variablesperfunction++;
                            stack_count = stack_count + 1;
                        }
                        for(int j=0; j<comma;j=j+1) //If it has a comma means it has more man 1 variable declared inside the function
                        {
                            token = strtok(NULL, ",");
                            array[stack_count].stack = MallocString(token);
                            if(array[stack_count].stackfunctions == NULL)
                                array[stack_count].stackfunctions = MallocString(array[stack_count-1].stackfunctions);
                            array[numberoffunction_count].variablesperfunction++;
                            stack_count = stack_count + 1;
                        }
                        function_check  = false;
                    }
                    else if(alloc_checker(line)) //if it a malloc/calloc in function sotre in heap
                    {
                        int d = 0;
                        token = strtok(line,";");
                        token = strtok(line," ");
                        char*temp = MallocString(token);
                        bool done = false;
                        for(d=0;d<7;d=d+4)
                        {
                            if(strncmp(Function_Names[d],line,strlen(Function_Names[d]))==0) //if int*i=malloc()
                            {
                                temp = strtok(temp,"=");
                                array[stack_count].stack = MallocString(temp);
                                if(array[stack_count].stackfunctions == NULL)
                                    array[stack_count].stackfunctions = MallocString(array[stack_count-1].stackfunctions);
                                stack_count = stack_count + 1;
                                array[numberoffunction_count].variablesperfunction++;
                                token = strstr(token,"*");
                                array[heap_count].heap = MallocString(token);
                                done = true;
                                break;
                            }
                        }
                        if(!done) //if i=malloc()
                        {
                            array[heap_count].heap = MallocString(token);
                        }
                        if(array[heap_count].heapfunctions == NULL)
                            array[heap_count].heapfunctions = MallocString(array[heap_count-1].heapfunctions);
                        heap_count = heap_count + 1;
                    }
                    else if(ro_checker(line)) //if it a string literal in function sotre in RO Data
                    {
                        token = strtok(line,";");
                        token = strtok(line," ");
                        array[ROData_count].ROData = MallocString(token);
                        if(array[ROData_count].RODatafunctions == NULL)
                            array[ROData_count].RODatafunctions = MallocString(array[ROData_count-1].RODatafunctions);
                        array[numberoffunction_count].variablesperfunction++;
                        ROData_count=ROData_count+1;
                    }
                    else if(strstr(line,";")!=NULL) //Any vaariable other than these one eg inti; or inti=5;
                    {
                        token = strtok(line,";");
                        token = strtok(line," ");
                        for (int i=0; i<7; i++)
                        {
                            if((strncmp(Function_Names[i],token,strlen(Function_Names[i]))==0))
                            {
                                if(comma>=1&&token[strlen(token)-1]!='}') //if th line has more than one integreal delcared once
                                {
                                    token = strtok(token, ",");
                                    array[stack_count].stack = MallocString(token);
                                    if(array[stack_count].stackfunctions == NULL)
                                        array[stack_count].stackfunctions = MallocString(array[stack_count-1].stackfunctions);
                                    array[numberoffunction_count].variablesperfunction++;
                                    stack_count = stack_count + 1;
                                    
                                    for(int j=0; j<comma;j=j+1)
                                    {
                                        token = strtok(NULL, ",");
                                        char*tempo = MallocString(Function_Names[i]+1);
                                        strcpy(tempo, Function_Names[i]);
                                        strcat(tempo, token);
                                        if(j+1==comma)
                                            tempo = strtok(tempo, "=");
                                        
                                        array[stack_count].stack = MallocString(tempo);
                                        if(array[stack_count].stackfunctions == NULL)
                                            array[stack_count].stackfunctions = MallocString(array[stack_count-1].stackfunctions);
                                        array[numberoffunction_count].variablesperfunction++;
                                        stack_count = stack_count + 1;
                                    }
                                    break;
                                }
                                else //if only 1 variable in delcared per line
                                {
                                    array[stack_count].stack = MallocString(token);
                                    if(array[stack_count].stackfunctions == NULL)
                                        array[stack_count].stackfunctions = MallocString(array[stack_count-1].stackfunctions);
                                    array[numberoffunction_count].variablesperfunction++;
                                    stack_count = stack_count + 1;
                                    break;
                                }
                            }
                        }
                    }
                    fgets(line,sizeof(line),file);
                    array[numberoffunction_count].linesperfumctiom++; //keep itterating on the while the function has ended
                    array[0].linescount++;
                    strcpy(ogline, line);
                    space_remover(line);
                }
                numberoffunction_count++;
            }
            else if(alloc_checker(line)) //if it a malloc/calloc as global variable sotred in heap
            {
                int d = 0;
                token = strtok(line,";");
                token = strtok(line," ");
                char*temp = MallocString(token);
                bool done = false;
                for(d=0;d<7;d=d+4)
                {
                    if(strncmp(Function_Names[d],line,strlen(Function_Names[d]))==0) //if int*i=malloc()
                    {
                        temp = strtok(temp,"=");
                        array[static_data_count].static_data = MallocString(token);
                        static_data_count = static_data_count + 1;
                        token = strstr(token,"*");
                        array[heap_count].heap = MallocString(token);
                        done = true;
                        break;
                    }
                }
                if(!done) //if i=malloc()
                {
                    array[heap_count].heap = MallocString(token);
                }
                if(array[heap_count].heapfunctions == NULL)
                    array[heap_count].heapfunctions = MallocString(array[heap_count-1].heapfunctions);
                heap_count = heap_count + 1;
            }
            
            else if(ro_checker(line)) //if it a string literal in global store in RO Data
            {
                token = strtok(line,";");
                token = strtok(line," ");
                array[ROData_count].ROData = MallocString(token);
                if(array[ROData_count].RODatafunctions == NULL)
                    array[ROData_count].RODatafunctions = MallocString(array[ROData_count-1].RODatafunctions);
                ROData_count=ROData_count+1;
            }
            else if(line[0]!='\n') //Any vaariable other than these one eg inti; or inti=5; in the global
            {
                token = strtok(line,";");
                token = strtok(line," ");
                comma = commacounter(line);
                for (int p=0; p<7; p++)
                {
                    if(comma>=1&&token[strlen(token)-1]!='}') //if th line has more than one integreal delcared once
                    {
                        token = strtok(token, ",");
                        array[static_data_count].static_data = MallocString(token);
                        static_data_count = static_data_count + 1;

                        for(int j=0; j<comma;j=j+1)
                        {
                            array[static_data_count].static_datafunctions = MallocString("global");
                            token = strtok(NULL, ",");
                            char*tempo = MallocString(Function_Names[p]+1);
                            strcpy(tempo, Function_Names[p]);
                            strcat(tempo, token);
                            if(j+1==comma)
                                tempo = strtok(tempo, "=");
                            array[static_data_count].static_data = MallocString(tempo);
                            static_data_count = static_data_count + 1;
                        }
                        break;
                    }
                    else //if only 1 variable in delcared per line
                    {
                        array[static_data_count].static_data = MallocString(token);
                        static_data_count = static_data_count + 1;
                        break;
                    }
                }
            }
        }
        printeOData(ROData_count,array); //Print all of the RO Data
        printf("\n### static data ###\n");
        for (int k=0; k<static_data_count; k=k+1)
        {
            print_staticNstack(array[k].static_data,array[k].static_datafunctions); //Print the static data
        }
        printf("\n### heap ###       scope  type  size\n");
        for (int m=0; m<heap_count; m=m+1)
        {
            printalloc(array[m].heap,array[m].heapfunctions); //Print the heap data
        }
        printf("\n####################\n### unused space ###\n####################\n"); //Print the unsued space
        printf("\n### stack ###\n");
        for (int l=0; l<stack_count; l=l+1) //Print the Stack Data
        {
            if(strncmp(array[l].stack,"char**argv",strlen("char**argv"))==0)
                printf("     argv            main  char**   %lu\n",sizeof(char**)); //if argv print one line
            else
                print_staticNstack(array[l].stack,array[l].stackfunctions); //else print all of the lines
        }
        printf("\n**** STATS ****\n");
        printf("  - Total number of lines in the file: %d\n",array[0].linescount); // Stat Printing
        printf("  - Total number of functions: %d\n   ",numberoffunction_count);
        for(int i=0;i<numberoffunction_count;i=i+1)
        {
            if(i+1==numberoffunction_count)
                printf(" %s\n",array[i].numberoffunction); //Last one don't add a comma
            else
                printf(" %s,",array[i].numberoffunction); //everything else print a camma at end
        }
        printf("  - Total number of lines per functions:\n");
        for(int i=0;i<numberoffunction_count;i=i+1)
        {
            printf("    %s: %d\n",array[i].numberoffunction,(array[i].linesperfumctiom-2)); //Print #of the lines per function
        }
        printf("  - Total number of variables per function:\n");
        for(int i=0;i<numberoffunction_count;i=i+1)
        {
            printf("    %s: %d\n",array[i].numberoffunction,array[i].variablesperfunction); //Print the number of variable per function
        }
        printf("//////////////////////////////\n");
    }
    return 0;
}


