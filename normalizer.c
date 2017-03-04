#include <stdio.h>
#include <string.h>

int main(int argc, char ** args){
	if(argc != 7 && argc != 5){
		printf("usage: normalizer -i instruction_file [-m memory_file] -o output_file");
		return 0;
	}

	int ins_idx = 0;
	int mem_idx = 0;
	int out_idx = 0;
	int i;
	for(i = 1;i < argc;i++){
		if(strcmp(args[i], "-i") == 0){
			ins_idx = i + 1;
		}
		if(strcmp(args[i], "-m") == 0){
			mem_idx = i + 1;
		}
		if(strcmp(args[i], "-o") == 0){
			out_idx = i + 1;
		}
	}

	// process instruction file
	FILE * fin = fopen(args[ins_idx], "r");
	FILE * fout = fopen(args[out_idx], "w");
	int seen = 0;
	int new_ins = 1;
	int label = 0;
	char c;
	while((c = fgetc(fin)) != EOF){
		if(c == ' ' || c == '\t' || c == '\n'){
			if(label){
				seen = 0;
			} else {
				seen = 1;
			}
		} else if('a' <= c && c <= 'z'){
			label = 0;
			if(new_ins == 0){
				if(seen){
					fputc(' ', fout);
					seen = 0;
				}
			}
			fputc(c + 'A' - 'a', fout);
			new_ins = 0;
			seen = 0;
		} else if(c == ';'){
			label = 0;
			seen = 0;
			new_ins = 1;
			fputc(c, fout);
			fputc('\n', fout);
		} else if(c == ':'){
			fputc(c, fout);
			new_ins = 0;
			seen = 0;
			label = 1;
		} else {
			if(new_ins == 0){
				if(seen){
					fputc(' ', fout);
					seen = 0;
				}
			}
			label = 0;
			fputc(c, fout);
			new_ins = 0;
			seen = 0;
		}
	}
	fputc('\n', fout);
	fclose(fin);

	// process memory file
	if(mem_idx){
		fin = fopen(args[mem_idx], "r");
		int state = 0;
		while((c = fgetc(fin)) != EOF){
			if(c == ' ' || c == '\t' || c == '\n'){
				if(state == 2){
					fputc('\n', fout);
					state = 0;
				}
			} else if('a' <= c && c <= 'z'){
				fputc(c + 'A' - 'a', fout);
				if(state == 1){
					state = 2;
				}
			} else if(c == ':'){
				fputc(c, fout);
				state = 1;
			} else {
				fputc(c, fout);
				if(state == 1){
					state = 2;
				}
			}
		}
		fclose(fin);
	}
	fclose(fout);
	return 0;
}