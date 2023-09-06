# include <stdio.h>
# include <math.h>
# include <ctype.h>
# include <stdlib.h>
# include <string.h>
# define MAXCHAR  1024	 
# define MAXCHAR2  10240	 
# define MAXFIELD 2000
# define debug 0

typedef struct {
	char field[MAXCHAR];
	char cat[20];
	int pid;
} FIELD;
 
FIELD field[MAXFIELD];
int nfield = 0;
int omit_field_id = -1;
char ifilename[MAXCHAR]; 
char ofilename[MAXCHAR]; 
char line[MAXCHAR2];
char line2[MAXCHAR2];
char omit_field[MAXCHAR];

FILE *fpin, *fpout;

int  main(int argc, char *argv[]) {
int  i,j;
int flag;
char tmpchar1[MAXCHAR];
char str[MAXCHAR2];

if(argc==2&&(strcmp(argv[1], "-h")==0||strcmp(argv[1], "-H")==0)) {
    printf("Usage: modify_fun_matlab  -i  input \n");   
    printf("                          -o  output\n");   
    printf("                          -id id of the variable to be removed\n");   
    exit(0);
  }   
if(argc != 7) {
    printf("Usage: modify_fun_matlab  -i  input \n");   
    printf("                          -o  output\n");   
    printf("                          -id id of the variable to be removed\n");   
}  
for(i=1;i<argc;i+=2) {
	if(strcmp(argv[i], "-i")==0) {
     		strcpy(ifilename, argv[i+1]);
     		continue;
	}
	if(strcmp(argv[i], "-o")==0) {
     		strcpy(ofilename, argv[i+1]);
     		continue;
	}
	if(strcmp(argv[i], "-id")==0) {
		omit_field_id = atoi(argv[i+1]) ;
     		continue;
	}
}
if((fpin=fopen(ifilename,"r"))==NULL) {
     	printf("\n Cannot open file %s, exit", ifilename);
	exit(1);
}
if((fpout=fopen(ofilename,"w"))==NULL) {
     	printf("\n Cannot open file %s, exit", ofilename);
	exit(1);
}
/* check how many fields and how many rows */
omit_field_id -= 1;
strcpy(omit_field, "NOT KNOW");

for (;;) {
       	if (fgets(line, MAXCHAR2, fpin) == NULL) break;
	strcpy(tmpchar1, "");
	tmpchar1[0]='\n';
	sscanf(line, "%s", tmpchar1);
	flag = 0;
	if(strcmp(tmpchar1, "predictorNames") == 0)  flag = 1;	
	if(strcmp(tmpchar1, "isCategoricalPredictor") == 0)  flag = 2;	
	if(strcmp(tmpchar1, "trainedModel.RequiredVariables") == 0)  flag = 3;	
	if(flag == 0) fprintf(fpout, "%s", line);
	if(flag == 1) {
		for(i=0; i<strlen(line); i++) {
			if(line[i] == '{') {
				line[i] = ' ';
				if(nfield==0) {
					field[nfield].pid = i; 
					nfield++;
				}
			}
			if(line[i] == '}') line[i] = ' ';
			if(line[i] == ',') {
				line[i] = ' ';
				field[nfield].pid = i; 
				nfield++;
				if(nfield >= MAXFIELD) {
					fprintf(stderr, "Too man fields, increase MAXFIELD and recompile the program\n");
					exit(1);
				}
			}
		}	
		sscanf(&line[field[omit_field_id].pid], "%s", omit_field);
		for(i=0; i<nfield; i++) {
			sscanf(&line[field[i].pid], "%s", field[i].field);
			if(debug == 1) printf("%5d %s\n", i+1, field[i].field);
		}
		if(omit_field_id < 0 || omit_field_id >= nfield) {
			fprintf(stderr, "The field ID must >= 1 and <= %d\n", nfield);
			exit(1);
		}
		printf("Omit Field %5d %s\n", omit_field_id+1, omit_field);
		fprintf(fpout, "%s", "predictorNames = {");
		if(omit_field_id == 0) {
			for(i=1; i< nfield-1; i++) 
				fprintf(fpout, "%s, ", field[i].field);
			fprintf(fpout, "%s", field[nfield-1].field);
		}
		else if (omit_field_id == (nfield -1)) {
			for(i=0; i< nfield-2; i++) 
				fprintf(fpout, "%s, ", field[i].field);
			fprintf(fpout, "%s", field[nfield-2].field);
		}
		else {
			for(i=0; i< nfield-1; i++) {
				if(i== omit_field_id) continue;
				fprintf(fpout, "%s, ", field[i].field);
			}
			fprintf(fpout, "%s", field[nfield-1].field);
		}
		fprintf(fpout, "%s\n", "};");
		nfield = 0;
	}
	if(flag == 2) {
		for(i=0; i<strlen(line); i++) {
			if(line[i] == '[') {
				line[i] = ' ';
				if(nfield==0) {
					field[nfield].pid = i; 
					nfield++;
				}
			}
			if(line[i] == ']') line[i] = ' ';
			if(line[i] == ',') {
				line[i] = ' ';
				field[nfield].pid = i; 
				nfield++;
				if(nfield >= MAXFIELD) {
					fprintf(stderr, "Too man fields, increase MAXFIELD and recompile the program\n");
					exit(1);
				}
			}
		}
		for(i=0; i<nfield; i++) {
			sscanf(&line[field[i].pid], "%s", field[i].field);
			if(debug == 1) printf("%5d %s\n", i+1, field[i].field);
		}
		fprintf(fpout, "%s", "isCategoricalPredictor = [");
                if(omit_field_id == 0) {
                        for(i=1; i< nfield-1; i++)
                                fprintf(fpout, "%s, ", field[i].field);
                        fprintf(fpout, "%s", field[nfield-1].field);
                }
                else if (omit_field_id == (nfield -1)) {
                        for(i=0; i< nfield-2; i++)
                                fprintf(fpout, "%s, ", field[i].field);
                        fprintf(fpout, "%s", field[nfield-2].field);
                }
                else {
                        for(i=0; i< nfield-1; i++) {
                                if(i== omit_field_id) continue;
                                fprintf(fpout, "%s, ", field[i].field);
                        }
                        fprintf(fpout, "%s", field[nfield-1].field);
                }
                fprintf(fpout, "%s\n", "];");
		nfield = 0;
	}
        if(flag == 3) {
                for(i=0; i<strlen(line); i++) {
                        if(line[i] == '{') {
                                line[i] = ' ';
                                if(nfield==0) {
                                        field[nfield].pid = i;
                                        nfield++;
                                }
                        }
                        if(line[i] == '}') line[i] = ' ';
                        if(line[i] == ',') {
                                line[i] = ' ';
                                field[nfield].pid = i;
                                nfield++;
                                if(nfield >= MAXFIELD) {
                                        fprintf(stderr, "Too man fields, increase MAXFIELD and recompile the program\n");
                                        exit(1);
                                }
                        }
                }
                for(i=0; i<nfield; i++) {
                        sscanf(&line[field[i].pid], "%s", field[i].field);
                	if(debug == 1) printf("%5d %s\n", i+1, field[i].field);
                }
		fprintf(fpout, "%s", "rainedModel.RequiredVariables = {");
		if(strcmp(omit_field, field[0].field) == 0) {
                        for(i=1; i< nfield-1; i++)
                                fprintf(fpout, "%s, ", field[i].field);
                        fprintf(fpout, "%s", field[nfield-1].field);
                }
		else if(strcmp(omit_field, field[nfield-1].field) == 0) {
                        for(i=0; i< nfield-2; i++)
                                fprintf(fpout, "%s, ", field[i].field);
                        fprintf(fpout, "%s", field[nfield-2].field);
                }
                else {
                        for(i=0; i< nfield-1; i++) {
				if(strcmp(omit_field, field[i].field) == 0) continue;
                                fprintf(fpout, "%s, ", field[i].field);
                        }
                        fprintf(fpout, "%s", field[nfield-1].field);
                }
		fprintf(fpout, "%s\n", "};");
		nfield = 0;
	}
   }
}
