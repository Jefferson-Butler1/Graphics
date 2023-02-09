#include <stdio.h>
#include <stdlib.h>

void println(char line[]){
	int i = 0;
	while(line[i] != ' '){
		printf("%c", line[i]);
		i++;
	}
	printf("\n");
}

int read_obj(FILE *f, FILE *out){
	int offset = -1;
	char read[50];
	int stop = 20;
	int line_length;
	fgets(read, 200, f);
	while(read[0] != 'v'){
		fgets(read, 200, f);
	}
	
	fputs("\n", out);
	fpos_t first_line;
	fgetpos(out, &first_line);
	fputs("                 ", out);
	fputs("\n\n", out);
	
	
	
	int point_count = 0;
	while(read[0] == 'v' && read[1] == ' '){
		read[0] = ' ';
		fputs(read,out);
		point_count++;
		fgets(read,200,f); 
	}
	
	fputs("\n", out);
	
	fpos_t poly_start;
	
	fgetpos(out, &poly_start);
	//return 0;
	
	fsetpos(out, &first_line);
	fprintf(out, "%i", point_count);
	
	fsetpos(out, &poly_start);
	
	fputs("                 ", out);
	fputs("\n\n", out);
	
	char read_char = ' ';
	while(read_char != 'f'){
		fscanf(f,"%c",&read_char);
	}
	
	//printf("found polys\n");
	
	
	fscanf(f,"%c",&read_char);
	
	int j = 0;
	int vertex_count = 0;
	int vertex_indices;
	int point_indices[100];
	int face_count = 0;
	while(!feof(f)){ //loop through all faces
		vertex_count = 0;
		//loop through single face
		
		int i = 0;
		while(read_char != '\n'){
			char poly_buffer[100] = {0};
			j = 0;
			int has_read_vertex = 0;
			
			fscanf(f,"%c", &read_char);
			while(read_char != ' '){ //loop through single vertex
				if(read_char == '\n'){break;}
				if(read_char == '/'){has_read_vertex = 1;}
				if(!has_read_vertex){
					poly_buffer[j] = read_char; j++; // add to poly buffer
				}
				
				
				fscanf(f,"%c", &read_char);
			}
			// add to array of ints
			// fput each int value into thing.
			//if(read_char == '\n'){continue;}
			point_indices[i] = atoi(poly_buffer);
			if(point_indices[i] + offset >= 0){i++; vertex_count++;}
 			poly_buffer[j] = ' ';
			j++;
			
		
		}
		/*
		char poly_buffer2[100] = {0};
		
		for(int i = 0; i < j-1; i++){
			poly_buffer2[i] = poly_buffer[i];
		}
		
		fprintf(out, "%i", vertex_count);
		fputs("     ", out);
		fputs(poly_buffer2, out);
		fputs("\n", out);
		*/
		face_count++;
		fprintf(out, "%i", vertex_count);
		fputs("     ", out);
		for(int j = 0; j < i; j++){
			fprintf(out, "%i", point_indices[j] + offset);
			fputs(" ", out);
		}
		fputs("\n", out);
		
		// put poly buffer into out file
		
		fscanf(f, "%c", &read_char);
		fscanf(f,"%c",&read_char);
		
		
		/*
		int face_count = 0;
		int j = 0;
		while(read[0] != 'f' && read[0] && !feof(f)){// loop through all points in a single poly
		//filter read down to simply vertex index
			printf("%i\n", read[0]);
			face_count++;
			for(int i = 0; read[i] != '\0'; i++, j++){ //loop through entire read string and read into poly buffer
				if(read[i] == '/'){poly_buffer[j] = ' '; j++; break;}
				poly_buffer[j] = read[i];
				j++;
				
			}
			//printf("end of vertex\n");
			fscanf(f, "%s", read);
		}
		//printf("end of line\n");
		fprintf(out, "%i", face_count);
		fputs("     ", out);
		fputs(poly_buffer, out);
		fputs("\n", out);
		
		*/
		
	}
	
	fsetpos(out, &poly_start);
	fprintf(out, "%i", face_count);
	
	printf("end of file\n");
	//fputs(read, out);
	//fputs("\n", out);
	
		
}

int read_faces(FILE *in, FILE *out){
	
	return 0;
}

int main(int argc, char *argv[]){

	if(argc != 3){printf("two arguments expected: obj file, output file name\n"); return 0;}
	
	
	FILE *fin = fopen(argv[1], "r");
	FILE *fout = fopen(argv[2], "w");
	
	read_obj(fin, fout);
	
	fclose(fin);
	fclose(fout);
	printf("end of file2\n");
}
