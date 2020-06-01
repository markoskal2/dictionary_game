#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<unistd.h>
#include<time.h>


#define WORDLEN 30
#define CHECK(X,Y){\
if(X==Y){\
	perror("error");\
	clearlist(X);\
	exit(1);\
	}\
}
	
typedef enum {SORTED,NOTSORTED} type;

typedef union DATA{
   int count;
   char *str;
} dataType; 

typedef struct list{
	dataType data;
	type list_type;
	struct list *nxt;
	struct list *prv;
}lista;


lista *func(int length,char dictionary[WORDLEN]);
char *getWord(char *filename);
int checkarv(int argc,char** argv,int* letters,int* steps);
void Print(lista* head);
void Insert(lista* head,lista* new_node,int dep);
lista *av_search(lista* head,lista* curr,lista* destination);
void clearlist(lista* head);
lista *remove_node(lista* head,lista* node);
lista *full_remove_node(lista* head,lista* node);


int main(int argc,char* argv[]){
	int letters=0,steps=0,i=0,y=0;
	lista* head,* prog,* temp,* curr,*start,*destination; //prog= path's head
	char play='y';
	srand((int)time(NULL));
	
	
	if(checkarv(argc,argv,&letters,&steps)==0){
		return(1);
	}
	
	/*create list with available words*/
	head=func(letters,argv[3]);
	if(head==NULL){
		printf("Not enough word to play the game\n");
		return(1);
	}
	printf("List of words = \n");
	Print(head);
	
	while(play=='y' || play=='Y'){
		/*choose random words*/
		i=0;
		for(curr=head->nxt;i<2;curr=curr->nxt){
			if(curr==head){
				continue;
			}
			if(rand()%2== 0 ){
				if(i==0){
					start=curr;
					i++;
				}else{
					if(start==curr){
						break;
					}
					destination=curr;
					i++;
					break;
				}
			}
		}
		
		printf("\tStarting word: %s\n\tTarget word: %s\n",start->data.str,destination->data.str);
		printf("Try to go from \"%s\" to \"%s\" in no more than %d steps\n",start->data.str,destination->data.str,steps);
		
		prog=(lista*) malloc(sizeof(lista));
		CHECK(prog,NULL);
		curr=(lista*) malloc(sizeof(lista));
		CHECK(curr,NULL);
		prog->data.count=0;
		prog->nxt=NULL;
		prog->prv=NULL;
		curr->data.str=start->data.str;
		
		Insert(prog,curr,SORTED);
		y=0;
		while(steps-y>0){
			printf("Progress so far: ");
			Print(prog);
			printf("Remaining steps: %d\n",steps-y);
			printf("Target: %s\n",destination->data.str);
			printf("\n");
			printf("Choose one of the following words: \n");
			temp=av_search(head,curr,destination);
			if(temp==NULL){
				break;
			}else if(strcmp(temp->data.str,destination->data.str)==0){
				printf("\nGOOD WORD!");
				Insert(prog,temp,SORTED);
				Print(prog);
				break;
			}
			curr=temp;
			Insert(prog,curr,SORTED);
			y++;
		}
		if(y==steps){
			printf("TOO BAD :( ");
			printf("Target was %s\n",destination->data.str);
			printf("A solution is...\n");
			
		}
		printf("Play again? (y/n):");
		scanf(" %c",&play);
		while(play!='y' && play!='Y' && play != 'n' && play != 'N'){
			printf("Please enter y for yes or n for no.\n");
			printf("Play again? (y/n):");
			scanf(" %c",&play);
		}
		while(remove_node(prog,prog->nxt)!=NULL){};
	}
	//Print(head);
	clearlist(head);
	//clearlist(prog);
	return(0);
}

/*search for available words for the next step*/
lista* av_search(lista* head,lista* curr,lista* destination){
	lista *node;
	lista* new;
	lista *lekseis;
	char epilogi[WORDLEN];
	//char *word;
	int y=0,x=0;
	int flag=0;
	

	lekseis=(lista*) malloc(sizeof(lista));
	CHECK(lekseis,NULL);
	
	lekseis->data.count=0;
	lekseis->nxt=NULL;
	lekseis->prv=NULL;
	
	for(node=head->nxt;node!=head;node=node->nxt){
		flag=0;
		if(strcmp(node->data.str,curr->data.str)==0){
			continue;
		}
		for(y=0;node->data.str[y]!='\0';y++){
			if((node->data.str[y]-curr->data.str[y])!=0){
				flag++;
				continue;
			}
		}
		if(flag==1){
			x++;
			
			new=(lista*) malloc(sizeof(lista));
			CHECK(new,NULL);
			new->data.str=node->data.str;
			Insert(lekseis,new,SORTED);
			printf("%s ",new->data.str);
			if((x+3)%3==0){
				printf("\n");
			}
		}
		
	}
	if(x==0){
		printf("There are no words to choose from. Sorry!\n");
		return(NULL);
	}
	printf("\n");
	printf("Enter Selection: \n");
	scanf(" %s",epilogi);
	
	
	for(node=lekseis->nxt;node!=lekseis;node=node->nxt){
		if(strcmp(epilogi,node->data.str)==0){
			new=(lista*) malloc(sizeof(lista));
			CHECK(new,NULL);
			new->data.str=(char *)malloc(strlen(epilogi)*sizeof(char));
			strcpy(new->data.str,epilogi);
			
			while(remove_node(lekseis,lekseis->nxt)!=NULL){};
			return(new);
		}
	}
	
	
	printf("ERROR: Your selection is not available. Try again\n");
	return(NULL);
}

/*this function finds every word which is in the correct length and makes a list*/
lista* func(int length,char dictionary[WORDLEN]){
	lista* head;
	lista* new;
	char *word;
	
	word=NULL;
	
	
	head=(lista*) malloc(sizeof(lista));
	CHECK(head,NULL);
	
	head->nxt=NULL;
	head->prv=NULL;
	head->data.count=0;
	
	do{
		word=getWord(dictionary);
		if(word==NULL){
			break;
		}
		
		if(strlen(word)==length){
			new=(lista*) malloc(sizeof(lista));
			CHECK(new,NULL);
			new->data.str=word;

			Insert(head,new,NOTSORTED);
		}
		
	}while(word!=NULL);
	
	
	if(head->data.count<2){
		clearlist(head);
		return(NULL);
	}
	return(head);
}

/*function for checking arguments*/
int checkarv(int argc,char** argv,int* letters,int* steps){
	int temp;
	
	if(argc<4){
		
		printf("Insufficient arguments.\n");
		printf("%s LENGTH STEPS DICTIONARY\n\tLENGTH: requested length of word.\n\tSTEPS: requested number of steps. Must be >0\n\tDICTIONARY: name of dictionary file\n",argv[0]);
		return(0);
	}else if(argc>4){
		printf("Too many arguments.\n");
		printf("%s LENGTH STEPS DICTIONARY\n\tLENGTH: requested length of word.\n\tSTEPS: requested number of steps. Must be >0\n\tDICTIONARY: name of dictionary file\n",argv[1]);
		return(0);
		
	}
	
	temp=atoi(argv[1]);
	*letters=temp;
	if(*letters<1){
		while(*letters<=1){
			printf("A word may not have fewer than one letters. Specify a different number of letters:\n");
			scanf(" %d",letters);
		}
	}
	
	
	temp=atoi(argv[2]);
	*steps=temp;
	if(*steps<=0){
		while(*steps<=1){
			printf("You may not have fewer than one steps. Specify a different number of steps:\n");
			scanf(" %d",steps);
		}
	}
	
	
	return(1);
}
/*function for printing the list*/
void Print(lista* head){
	lista* node;
	
	for(node=head->nxt;node!=head;node=node->nxt){
		printf(" %s ",node->data.str);
		if(node->nxt!=head){
			printf("-->");
		}
	}
	printf("\n");
	return;
}
/*function from Insertion to double linked list.Variable "dep" stands for sorted-notsorted
(each time we choose as we like)*/
void Insert(lista* head,lista* new_node,int dep){
	lista* node;
	
	if(head->nxt==NULL){
		head->nxt=new_node;
		head->prv=new_node;
		new_node->nxt=head;
		new_node->prv=head;
		new_node->list_type=dep;
		head->data.count++;
		return;
	}
	
	if(dep==SORTED){
		for(node=head->nxt;node->nxt!=head;node=node->nxt){
				if(strcmp(new_node->data.str,node->data.str)<0){
					break;
				}
		}
		if(strcmp(new_node->data.str,node->data.str)<0){
			new_node->nxt=node;
			new_node->prv=node->prv;
			new_node->prv->nxt=new_node;
			node->prv=new_node;
			new_node->list_type=dep;
			head->data.count++;
			return;
		}
		new_node->list_type=dep;
		new_node->nxt=node->nxt;
		new_node->prv=node;
		new_node->nxt->prv=new_node;
		node->nxt=new_node;
		head->data.count++;
		
	}else{
		for(node=head->nxt;node->nxt!=head;node=node->nxt);
		new_node->nxt=node->nxt;
		new_node->prv=node;
		new_node->nxt->prv=new_node;
		node->nxt=new_node;
		new_node->list_type=dep;
		head->data.count++;
	}
	return;
}

/*using function remove_node we free all the dynamic allocated memory*/
void clearlist(lista* head){
	while(full_remove_node(head,head->nxt)!=NULL){};
}
/*function for removing a node from list(without freeing the str pointer)*/
lista* remove_node(lista* head,lista* node){
	lista *curr;
	
	if(node==head){
		if(head->data.count!=0){
			printf("Cannot delete sentinel ,list isn't empty\n");
			return(head);
		}
		free(head);
		head=NULL;
		return(head);
	}
	for(curr=head->nxt;curr!=head;curr=curr->nxt){
		if(curr==node){
			curr->prv->nxt=curr->nxt;
			curr->nxt->prv=curr->prv;
			free(curr);
			curr=NULL;
			head->data.count--;
			break;
		}
	}
	return(head);
}

/*function for removing a node from list*/
lista* full_remove_node(lista* head,lista* node){
	lista *curr;
	
	if(node==head){
		if(head->data.count!=0){
			printf("Cannot delete sentinel ,list isn't empty\n");
			return(head);
		}
		free(head);
		head=NULL;
		return(head);
	}
	for(curr=head->nxt;curr!=head;curr=curr->nxt){
		if(curr==node){
			curr->prv->nxt=curr->nxt;
			curr->nxt->prv=curr->prv;
			free(curr->data.str);
			free(curr);
			curr=NULL;
			head->data.count--;
			break;
		}
	}
	return(head);
}



char *getWord(char *filename)  {
	char formatstr[15], *word;
	static FILE *input;
	static int firstTime = 1;
	if (firstTime) { 
		input = fopen(filename, "r");
		if (input == NULL) {
			printf("ERROR: Could not open file \"%s\"\n", filename);
			exit(1);
		}
		firstTime = 0;
	}
	word = (char*)malloc(sizeof(char)*WORDLEN);
	if (word == NULL) {
		printf("ERROR: Memory allocation error in getWord\n");
		exit(1);
	}
	sprintf(formatstr, "%%%ds", WORDLEN-1);
	fscanf(input, formatstr, word);
	if (feof(input)) {
		fclose(input);
		firstTime = 1;
		return NULL;
	}
	
	return word;
}