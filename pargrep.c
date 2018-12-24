#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define buff 1024
int count_thread=0;
//static int size_of_file;
const char *targetString_G   = NULL;

struct struct_def {
	int tid;
    char *fname;
    //int   outputPath;
    long  start;
    long  end;
    char *buf;
};

void *thread_def(void *arg){
	int line_number=0;
	struct struct_def *file=(struct struct_def*) arg;


	//pthread_t thId = pthread_self();
	FILE *fp_status;
	    long  line_subtract = 0;
	    long  Size_for_each_thread = file->end - file->start;
	   // printf("size for thread %ld",Size_for_each_thread);
	    long  temp  = Size_for_each_thread;

	    //char  buf[Size_for_each_thread * 4];
	    int buffSize=Size_for_each_thread*sizeof(char);
	    char *buf = (char*)calloc(buffSize,sizeof(char));
	    file->buf=(char*)calloc(Size_for_each_thread,sizeof(char));
	    //to allocate dynamically multiplied by random number 4

	    if(!(fp_status = fopen(file->fname, "r"))) {
		    printf("Error: File open failed : %s\n", file->fname);
	        return NULL;
	    }
	    //printf("Thread executing number : %d \n",count_thread);
	   // count_thread+=1;

	    // Starting from the specified point.
	    if (fseek(fp_status, file->start, SEEK_SET) != 0) {
	        fclose(fp_status);
	        return NULL;
	    }

	    while (temp > 0 && fgets(buf, buffSize, fp_status)) {
	    	line_number+=1;
	        if (strstr(buf, targetString_G) != NULL) {
	        	//printf("%d\t%s ",line_number, buf);
	        	strcat(file->buf,buf);


	        }
	        line_subtract = strlen(buf);
	        temp -= line_subtract;
	    }
	    fclose(fp_status);


		return NULL;

}

int main(int argc, char **argv)

{
	int not;

		//FILE *fp, *instream;
		const char *filename;
	if(argc == 2){
			if(strncmp(argv[1],"--help",7) == 0){  //If argument passed is help

	                        printf("Usage: %s\n\n",argv[0]);
	                        printf("./a.out   <options>\n");
	                        printf("Description:\n");
	                        printf("<Command> -t <number of threads> word file ");
	                        printf("\t --help \t Display this help and exit \n");
	                        return 0;
	}
			else {
				size_t content_size = 1024;	// if you provide input in format cat <file> | ./pargrep <word>

				void *content = calloc(buff,sizeof(buff));
					FILE *fp = fopen("input.txt","w");
					size_t read;
					while((read = fread(content,1,buff,stdin))){
						fwrite(content,read,1,fp);
						//printf("%s",content);

					}
					fclose(fp);
					filename = "input.txt";

					not=1;
					targetString_G = argv[1];
					//return -1;

			}
	}
	else if(argc < 2){
		printf("Too few arguments. \n You could check the --help option for help with the command");
	}
	else {
		if(argc >3 ){			//when -t optio	n is passed with the number of threads
					not=atoi(argv[2]);
					filename=argv[4];
					targetString_G = argv[3];
				}
				else{					// when you pass ./pargrep <word> <file>
					filename=argv[2];
					targetString_G = argv[1];
					not=1;

				}
	}

	 FILE*  fp     = fopen(filename,"r");
				 if(!fp){											//if file does not exist
					 printf("%s does not exist",filename);
					 return -1;
				 }

	pthread_t thr[not];


	/*struct stat buf;
	stat(filename, &buf);
	off_t size = buf.st_size;*/



	 int  size;
	 fseek(fp, 0L, SEEK_END);
	 size = ftell(fp);

	 int    i      = 0;
	int    j = 0;
	//printf("size of life %d",size);
	long   chunkSize =  size / not;
	struct  struct_def struct1[not];

	for (i = 0; i < not ; i++) {
	        // each chunk size
	        struct1[i].fname       = (char *)filename;
	        struct1[i].start       = i       * chunkSize + j;
	        struct1[i].end         = (i + 1) * chunkSize - 1 ;


	        // ifnext line move the pointer becuase it can split  a single line too
	        fseek(fp, struct1[i].end, SEEK_SET);
	        for (j = 0; fgetc(fp) != '\n'; j++ );

	        struct1[i].end += j ;


	        pthread_create(&thr[i], NULL,thread_def, (void *)&struct1[i]);
	    }


	    // added this to avoid segmentation fault core dumped
	    /*struct1[not - 1].fname      = (char *) filename;
	    struct1[not - 1].start      = (not - 1) * chunkSize + j;
	    struct1[not - 1].end        = size - 1;
	  //  arg[1000- 1].outputPath = 0;
	    pthread_create(&thr[i], NULL, thread_def, (void *)&struct1[i]);

*/	    // The file descriptor is only used in main thread.
	    // Thus, close it without influence for other threads.
	    fclose(fp);
	    void *buf;

	    for (i = 0; i < not; i++) {
	        pthread_join(thr[i],NULL);
	        printf("%s",struct1[i].buf);
	    }


return 0;

}

