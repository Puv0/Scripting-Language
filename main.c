#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <unistd.h>
#include <sys/stat.h>
#define max_size 100

#define FILENAME_MAX  100
#define WINDOWS
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

int main() {

		FILE *fp,*check ;

	int c,c1 ;
	char* temp = malloc(sizeof(char)*4);
	int length = 0 ;
	int counter = 0;
	int current_index = 0 ;
	int succes = 0 ;

	char* getcurrentPath(){


	char* buff=malloc(sizeof(char)*max_size);
    GetCurrentDir( buff, FILENAME_MAX );
    printf("Current working dir: %s\n", buff);
	return buff;
	}


	int check_endofline(FILE *fp,int current_index) //END OF LINE CHECK
	{
		int valid = 0;
		int pt_counter = 1 ;
		check = fp ;
		c1=fgetc(check);
		while(1){
			c1=fgetc(check);
			pt_counter ++;
			if(c1=='>'){
				c1=fgetc(check);
				pt_counter ++;
				while(isspace(c)!=0){
					c1=fgetc(check);
					pt_counter ++;
				}
				if(c1==';'){
				    valid = 1 ;
					break;;
				}
				else{
					break;
				}
			}
		}
		fseek(check,-pt_counter,SEEK_CUR);
		return valid;
	}

	void go(char* path){
		printf("PATH(%s)",path);
		succes = 0 ;
		if (strcmp(path,"*")==0)
		 {
		  if(chdir("..")==0)
		 	{
		 		succes = 1 ;
			}
			 else{

			 	printf("Can't go back");
			 }
		}
		else{

		if(chdir(path)==0){
			succes = 1 ;
		}
		else{
			printf("%s ",path);
			perror("This path doesn't exist");
			succes = 0;

		}

		}
		if(succes){

			printf("Go to : ");
			printf("%s ",path);
			getcurrentPath();

		}
	}


	void make(char* path){

		int control = mkdir(path);
		if(control!=-1){
			printf("Succes make (%s)\n",path);
		}
		else{
				printf("The file have already exist (%s)\n",path);

		}
	}

	char* getfileName(){  //take input for filename and add .pmk.txt for open to file.
	printf("What is your file name?: (Without pmk or txt)\n");
	 char *filename = malloc(sizeof(char)*max_size);
	fgets(filename,max_size,stdin);
	int size_input = strlen(filename);
	filename[size_input-1]=NULL;
	char txt[] =".pmk.txt";

	strncat(filename,txt,8); //MAKE file name.
	size_input = strlen(filename);

	filename[size_input]='\0';
	return filename;
	}

	const char *filename = getfileName();

		printf("Welcome now you are in: %s\n\n", getcurrentPath());

	char* get_current(){  //get current direction
	char* current_path=malloc(sizeof(char)*max_size);

	current_path = getcurrentPath();
	char* current_directory = malloc(sizeof(char)*max_size); //HOLD Last  DIRECTORY
	int k,n = 0 ;
	int slash_index = 0 ;
	k = strlen(current_path);
	int x = 0 ;
	while(1){
		k--;
		if(current_path[k-1]=='\\'){
			slash_index = k-1 ;
			for(n=k; n<strlen(current_path);n++){

				current_directory[x] = current_path[n];
				x++;

			}
			memset(current_path+slash_index,0,x);
			current_directory[x]='\0';

			free(current_path);
			return current_directory;

		}
	}

	}

	fp = fopen(filename,"r" );


	if(fp!=NULL){

	while(1){
		c= fgetc(fp);
		current_index++;

		if(isdigit(c)>0 || isalpha(c)>0 || c=='_' ) //Decide which command can we use
      {
            temp[counter]=c;
            counter++;
            length++;
            if(sizeof(temp)<length){
			temp = (char*)realloc(temp, sizeof(char)*(length+4));
			}

      }

	  else if(ispunct(c)!=0){    //Make the command and decide which type command is

		  temp[counter]='\0';
      	  int index = 0;
		  char* path = malloc(sizeof(char)*max_size);
		  int go_back = 0 ;


		  if(strcmp(temp,"make")==0) //MAKE SECTION
		  {
      		int valid = check_endofline(fp,current_index);
      		 printf("Command (%s)\n",temp);

			 if(valid!=1){
      			printf("There is no endofline in this line\n");
			  }


			 else{

		  	if(c=='<'){
		  		while(1){
		  			c=fgetc(fp);
					  if(c!='/' & c!='>'){

						path[index++]=c;
					  }

					  else{

					  	path[index]='\0';
					  	if(strcmp(path,"*")==0){
							  go(path);
						  }

					  	else if(c=='/'){
					  		make(path);
						  	go(path);
						  	go_back++;
						  }
						  else{
						  	make(path);
						  }
					  	memset(path,0,index);
					  	free(path);
					  	path=malloc(sizeof(char)*max_size);
					  	index=0;
					  	if(c=='>'){
							  int  j ;
							  for(j = 0 ; j<go_back;j++){
							  	go("*");
							  }
							  go_back = 0 ;
							  break;
						  }
					  }
				  }
			  }
		  }
	}


		  else if(strcmp(temp,"go")==0)  //GO SECTION
		  {

			  printf("Command (%s)\n",temp);

			  int valid = check_endofline(fp,current_index);

			if(valid!=1){
      			printf("There is no endofline in this line\n");
      			break;
			  }

			  else{

				if(c=='<'){
				while(1){

				c=fgetc(fp);
				while(isspace(c)!=0){
					c=fgetc(fp);
				}

				if(c!='/' & c!='>'){
					  path[index++]=c;

				  }

				else{
					path[index]='\0';
					go(path);
					if(succes !=1){
						break;
					}
					memset(path,0,index);
					free(path);
					path=malloc(sizeof(char)*max_size);
					index = 0 ;
					if(c=='>'){
						break;
					}
				}

			}

			}
		  }
		  }

		  else if(strcmp(temp,"if")==0){ //IF SECTION

		  	printf("Command (%s)\n", temp);
		  	int skip = 0 ;
		  	int go_back = 0 ;
		  	int k = 0 ;
			  if(c=='<'){
				while(skip!=1){

				c=fgetc(fp);
				while(isspace(c)!=0){
					c=fgetc(fp);

				}
				if(c=='*'){
					char* control_back = malloc(sizeof(char)*max_size);
					control_back = get_current();
					if(strcmp(tolower(control_back),"user") == 0){
						printf("cant go back");
						free(control_back);
						succes = 0 ;
						while(1){
					  	c=fgetc(fp);
					  	if(c=='{'){
					  		while(1){
					  			c=fgetc(fp);
					  			if(c=='}'){
					  			skip = 1 ;
								  	break;
								  }
							  }
						  	break;
						  }
					  	if(c==';'){
							  skip = 1 ;
							  break;
						  }
					  }
					}
					else{
						skip =1 ;
					}

					}


				  else if(c!='>' & c!='/'){
			  		path[index++]=c;

				  }

				else{
					path[index]='\0';
					go(path);
					if(succes!=1)
					{
					  while(1){
					  	c=fgetc(fp);
						if(c=='{'){
							while(1){
								c=fgetc(fp);
								if(c=='}'){
									skip = 1 ;
									break;
								}
							}

							break;
						}
					  	if(c==';'){
							  skip = 1 ;
							  break;
						  }
					  }
						for(k=0;k<go_back;k++){
								go("*");
								}

					}
					else{
						go_back++;
					}
					memset(path,0,index);
					free(path);
					path=malloc(sizeof(char)*max_size);
					index = 0 ;

					if(c=='>'){
						for(k=0;k<go_back;k++){
							go("*");
						}
						break;
						}

					}


				}


			}

			}


		  else if(strcmp(temp,"ifnot")==0){ //IFNOT SECTION

	   		printf("Command (%s)\n", temp);
		  	int skip = 0 ;
		  	int go_back = 0 ;
		  	int k = 0 ;
			  if(c=='<'){
				while(skip!=1){

				c=fgetc(fp);
				while(isspace(c)!=0){
					c=fgetc(fp);

				}
				if(c=='*'){
					char* control_back = malloc(sizeof(char)*max_size);
					control_back = get_current();
					if(strcmp(tolower(control_back),"user") != 0){
						free(control_back);
						succes = 1 ;
						while(1){
					  	c=fgetc(fp);

					  	if(c==';' || c=='}'){
							  skip = 1 ;
							  break;
						  }
					  }
					}
					else{
						skip =1 ;
					}

					}


				  else if(c!='>' & c!='/'){
			  		path[index++]=c;

				  }

				else{
					path[index]='\0';
					go(path);
					if(succes==1)
					{

						go_back++;
					  while(skip!=1){
					  	c=fgetc(fp);
					  	 if(c=='{'){
					  		while(1){
					  			c=fgetc(fp);
					  			if(c=='}')
					  			{
					  				skip = 1;
					  				for(k=0;k<go_back;k++){
					  					go("*");
									  }
									  break;
								  }
							  }

						  }
					  	else if(c==';'){
							  skip = 1 ;
							  for(k=0;k<go_back;k++){
								go("*");
								}
							  break;
						  }

					  }
					}

					memset(path,0,index);
					free(path);
					path=malloc(sizeof(char)*max_size);
					index = 0 ;

					if(c=='>'){
						for(k=0;k<go_back;k++){
								go("*");
								}
						break;
						}


					}


				}


			}

		  }

	  length = 0 ;
	  memset(temp,0,counter);
	  free(temp);
	  temp = malloc(sizeof(char)*4);
	  counter = 0 ;

	  }

		if(feof(fp)){
			break;
		}
}
	}


else{
	printf("File cannot open");
}
fclose(fp);
printf("\n");
getcurrentPath();

	return 0;

}
