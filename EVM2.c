#include<stdio.h> //Used for printf() and scanf()
#include<conio.h> //Used for clrscr() and getch()
#include<stdlib.h> //Used for exit()
#include<math.h> //Used for pow()
#include<string.h>
#include <unistd.h>

//File Opening

 //you cannot initialize global file pointers in C

//Implement a list for party to read and w
FILE* PartyList;
FILE* tempFile;
FILE* Record;

//Structure Definition

struct Voter {
	int voterId;
	char name[50];
	int hasVoted;
};

struct Party {
	char name[50];
	int noOfSeatsWon;
} partyList[10];
//strcpy(AAP.name,"AAP"); this statement is invalid strcpy works only within a function hence we have initialized the names in a main function

struct Candidate {
	char name[50];
	struct Party party;
	int noOfVotes;
};

struct Constituency {
	struct Voter voterList[100];
	struct Candidate candidateList[10];
	int id;
	int noOfVoters;
	int noOfCandidates;
	char name[50];
	int stopPolling;
	char winnerParty[50];
} temp;

//Global Variable

const int MAX = 2147483647;

int location; //for storing location of the record;
int id; //for voterId input
int cId; // constituencyId input
//int i; //for loop counter
int ch; //for loop choice
char fileName[26]; //for storing the name of temporary Party list 



//Function prototypes

int noOfRecord();
int checkFile(FILE* record);
int makeConstituencyId();
void writeConstituency(struct Constituency* New);
void addConstituencyDetails();
void updateConstituencyDetails();
void displayVoterList();
void vote();
int togglePolling(int recLocation);
void showConstituencyResults();
void displayCandidateList(int mode);
int deleteDetails();
void modifyRecord(struct Constituency *New,int locationIndex);
void readRecord(int locationIndex);
void selectCandidateParty(int startingIndex);
void checkInput(int *input,int upper, int lower);
void copyFile();
void initializePartyList();
void deleteParty(int index,int startingIndex);
void updateResult(int mode);
int findWinner();
void initializeResult();
void clearRecords();
void showAll();
void deleteFile(char* file);
int choice();
int menu(char *menuList,int lower, int upper,int strlen);

//Function definitions

//returns the no of constituencies in file
int noOfRecords() {
	Record = fopen("Records.dat","rb");
	fseek(Record,0,SEEK_END);
	return ftell(Record)/sizeof(struct Constituency);
	fclose(Record);
}

//Function to check if the file can be read or written
int checkFile(FILE* record) {
	if(record==NULL) {
		printf("\nError reading file! \a");
		return 1;
	}
	return 0;
}

//deletes a file using command line with the help of system()
void deleteFile(char* file) {
	char rmv[50] = "del /f ";
	strcat(rmv,file);
	system(rmv);
}


//initializes the filename where the list of available parties for candidates are stored.
void initializePartyList() {
	snprintf(fileName, 26, "PartyList-temp-%d.txt", temp.id);	
}
void copyFile(char* Source,char* Dest) {
	FILE* SourceFp = fopen(Source,"r");	
	FILE* DestFp = fopen(Dest,"w");
	checkFile(SourceFp);	
	checkFile(DestFp);		
	//FILE* newPartyListName = fopen(fileName,"w"); //write mode overwrites the file hence a seperate file pointer needs to be created
	
	char ch;
	
	while((ch = fgetc(SourceFp)) != EOF) {		
		fputc(ch,DestFp);
	}
	fclose(SourceFp);
	fclose(DestFp);
}

int choice() {
	int ch;
	printf("\nDo you want to proceed. Press ENTER to confirm...");
	ch = getch();
	if(ch == 13) {
		return 1;
	}
	else {
		return 0;
	}
}

void checkInput(int *input,int upper, int lower) {
	int j = 0,checkDecimal;
	do {
		if(j == 1) {
			printf("\nIncorrect input. Please Enter again : ");
		}
		fflush(stdin);
		//scanf returns the no of arguments (here 1) put into the formatted string if successful
		//if checkDecimal = 1 that means it is an integer 
		//if checkDecimal = 0 that means it is not an integer the checkdecimal
		checkDecimal = scanf("%d",input);
		j = 1;			
	} while(*input<lower || *input>upper|| checkDecimal == 0);			
}

//function to check the presence of the record and returns its location
int locateRecord(int cId) {	
	Record = fopen("Records.dat","rb");
	if(checkFile(Record) == 1) {
		return -1;
	}
	
	int recordLocationIndex = 0;
	while(fread(&temp,sizeof(temp),1,Record)) {
		if(temp.id==cId) {
			return recordLocationIndex;
		}
		recordLocationIndex++;
	}
	return -1;
	fclose(Record);
}

//Read the record and store it in temp variable
void readRecord(int locationIndex) {
	Record = fopen("Records.dat","rb");
	fseek(Record,locationIndex*sizeof(temp),SEEK_SET);
	fread(&temp,sizeof(temp),1,Record);
	fclose(Record);
}

//Replacing an existing record with new one
void modifyRecord(struct Constituency *New,int locationIndex) {	
        Record = fopen("Records.dat","r+b");
		fseek(Record,locationIndex*sizeof(struct Constituency),SEEK_SET);
		int write = fwrite(New,sizeof(*New),1,Record);	
		//printf("\nwrite status = %d",write);
		fclose(Record);	
}

//Generate a new constituency ID
int makeConstituencyId() {
        Record = fopen("Records.dat","rb");						
    	if (noOfRecords() == 0) {
        	return 1;
    	}  
		//read the last record and find its cId and add 1 to it to generate new cId 		 	
    	fseek(Record,-sizeof(temp),SEEK_END);
		if(fread(&temp,sizeof(temp),1,Record) == 0) {
			printf("\nCould not read the file !");
		}	
		
		printf("Generated Constituency ID: %d",temp.id + 1);
		return temp.id + 1;
		fclose(Record);					
}
//mode = 0 for reset
void updateResult(int mode) {
	int i;
	FILE* finalResult = fopen("Result.dat","wb");	
	FILE* pList = fopen("PartyList.txt","r");	
	checkFile(finalResult);
	checkFile(pList);	
	if(mode == 0) {
		if(choice()==0) {
			printf("\nResults not reset...");
			return;
		}
		for(i = 0; fscanf(pList,"%s",partyList[i].name) != EOF; i++){
			partyList[i].noOfSeatsWon = 0;
			//printf("\nnoofseats = %d, name = %s",partyList[i].noOfSeatsWon,partyList[i].name);
		}
		printf("\nResults have been reset...");
		getch();			
	}
	else {
		for(i = 0;i<10;i++) {
			if(strcmp(partyList[i].name,temp.winnerParty) == 0) {
			//fseek(finalResult,-prevPointer,SEEK_CUR);
			//printf("\npointer after going back at start of rec  = %d",ftell(finalResult));							
				partyList[i].noOfSeatsWon += mode;
			//	printf("\nnoofseats = %d, name = %s",partyList[i].noOfSeatsWon,partyList[i].name);
				break;
			}							
		}		
	}
	int write = fwrite(&partyList,sizeof(struct Party),10,finalResult);	
	//printf("\nwrite status = %d",write);
	fclose(finalResult);	
	fclose(pList);	
}

void initializeResult() {
	FILE* finalResult = fopen("Result.dat","rb");
	checkFile(finalResult);
	int read = fread(partyList,sizeof(struct Party),10,finalResult);
	//printf("\n Read status of initialize Result = %d",read);
	fclose(finalResult);
}

//Function to stop or start Polling
int togglePolling(int recLocation) {
	int toggle;
	if(temp.stopPolling == 0) {
		printf("\nDo you want to end the polling..");		
	}
	else {
		printf("\nDo you want to resume the polling..");					
	}
	
	if(choice() == 1)	{		
		if(findWinner() == 1) { //polling will not stop until atleast one vote is registered
			return 1; //unsuccessful
		}	
		if(temp.stopPolling == 0) {//resume to stop
			updateResult(1);
			temp.stopPolling = 1;
		}
		else {//stop to resume
			updateResult(-1);
			strcpy(temp.winnerParty,"");
			temp.stopPolling = 0;
		}	
		//temp.stopPolling = !temp.stopPolling;
		//printf("\nstopPolling = %d",temp.stopPolling);		
		modifyRecord(&temp,recLocation);
		return 0;
	}
	return 1;//unsuccessful	
}

void declareResult() {		
	FILE* newRecord = fopen("Records.dat","rb");	//had to create new file pointer as using 'Record' is buggy
	checkFile(newRecord);
	
	int max = 0,index,i;
	//struct Constituency temp2;
	if(noOfRecords() == 0) {
		printf("\nThere are no constituencies registered...");
		return;
	}
	i= 0;
	while(fread(&temp,sizeof(temp),1,newRecord)) {
		//printf("file pointer points to start of %d constituency", ftell(newRecord)/sizeof(temp) + 1);
		if(temp.stopPolling == 0) {
			printf("\nPolling for Constituency Id = %d has not stopped. Please stop it to continue....",temp.id);
			if(togglePolling(i) == 1) {
				printf("\n You have opted not to stop the polling hence result cannot be declared! Exiting...");
				return;
			}			
		}
		i++;
	}
	for(i = 0;i<10;i++) {
		if(partyList[i].noOfSeatsWon> max) {
			max = partyList[i].noOfSeatsWon;
			index = i;
		}
	}
	printf("\n'%s' has won the election with %d seats\n",partyList[index].name,partyList[index].noOfSeatsWon);
	fclose(newRecord);
}

//write the record in the file
void writeConstituency(struct Constituency* New) {	
    Record = fopen("Records.dat","r+b");	
	fseek(Record,0,SEEK_END);	
	if(fwrite(New,sizeof(struct Constituency),1,Record) == 0) {
		printf("\nError writing to the records file!");		
	}	
	fclose(Record);
}


void deleteParty(int index,int startingIndex) {			
	char party[50];	
	rewind(PartyList);				
	tempFile = fopen("temp.dat","w");
	int i=0;
	while(fscanf(PartyList,"%s",party)!=EOF) { //File reading does not contain \n
		i++;		
		if(i == index) {
			strcpy(temp.candidateList[startingIndex].party.name,party);			
			continue;
		}
		fprintf(tempFile,"%s\n",party);		
	}
	//getch();	
	fclose(tempFile);
	fclose(PartyList);	
	remove(fileName);
	rename("temp.dat",fileName);	
}

void selectCandidateParty(int startingIndex) {	
	int j;
	PartyList = fopen(fileName,"r+");
	
	if (checkFile(PartyList) == 1) {
		return;
	}
	
	char party[50]; 
		
		printf("\nSelect Party for %d candidate.",startingIndex + 1);
		for(j = 1; fscanf(PartyList,"%s",party)!=EOF ;j++) {				
				printf("\n%d. %s",j,party);	
				//printf("\nfile pointer after reading record = %d",ftell(PartyList));				
		}		
		
		printf("\nEnter your choice: ");
		//printf("\nj =%d ",j);		
		checkInput(&ch,j-1,1);
		
		/*rewind(PartyList);		
		for(i = 0;i<ch ; i++) {
			fscanf(PartyList,"%s %d",temp.candidateList[startingIndex].party.name,&noOfSeats);	
		}*/
		//printf("\nFile Pointer - %d",ftell(PartyList));	
		deleteParty(ch,startingIndex);				 //-1 is just a placeholder	
		printf("\nYou have selected '%s' party..",temp.candidateList[startingIndex].party.name);
		
		
}

void showAll() {
	int i;	
	printf("\ntemp.id = %d\nnofvoters = %d \n noof candidates = %d\n stoppolling = %d\n winner party = %s",temp.id, temp.noOfVoters,temp.noOfCandidates,temp.stopPolling,temp.winnerParty);
	for(i = 0;i<10;i++) {
		printf(" %s  %d",partyList[i].name,partyList[i].noOfSeatsWon);
	}
}


//Receive the constituency details from the user

void addConstituencyDetails() {
	int i;
	FILE* PartyName = fopen("PartyList.txt","r");
	FILE* newPartyListName = fopen(fileName,"w");
	//FILE* PartyList = fopen("PartyList.txt","r+");
	system("cls");		
	
	fflush(stdin);
	printf("\nEnter the name of the constituency : ");
	fflush(stdin);
	scanf("%s",&temp.name);
	
	temp.id =  makeConstituencyId();
	
	printf("\nEnter the no of voters you want to add(min = 2 max = 100) : ");	
	checkInput(&temp.noOfVoters,100,2);		
	
	temp.stopPolling = 0;
	temp.voterList[temp.noOfVoters].voterId = 0;
	strcpy(temp.winnerParty,"");
		
	for(i=0; i<temp.noOfVoters; i++){
		fflush(stdin);
		printf("\nEnter the name of the voter no - %d: ",i + 1);
		gets(temp.voterList[i].name);
		temp.voterList[i].voterId = temp.id*1000 + (i + 1);
		printf("\nVoter ID Generated for the voter: %d", temp.voterList[i].voterId);
		temp.voterList[i].hasVoted = 0;		
	}
	getch();
	fflush(stdin);
	system("cls");
	
	printf("\nEnter the no. of candidates you want to add(min = 2 max = 10) : ");	
	checkInput(&temp.noOfCandidates,10, 2);
	
	initializePartyList();
	copyFile("PartyList.txt",fileName);//copy contents of PartyList.txt to PartyList-temp.txt	
	
	for(i=0; i<temp.noOfCandidates; i++){
		fflush(stdin);
		printf("\nEnter the name of the candidate no. %d : ",i + 1);
		gets(temp.candidateList[i].name);
		temp.candidateList[i].noOfVotes = 0;
		selectCandidateParty(i);		
	}		
	fclose(PartyName);
	fclose(newPartyListName);
}

//Add new voter, candidate to the existing constituency 
void updateConstituencyDetails() {
	int i,newId;	
	char menuList4[3][18] = {"Add new Voter","Add new Candidate","Exit"};
	//printf("\n%d",temp.stopPolling);
	//check if the polling is stopped
	if(temp.stopPolling == 1) {
		if(togglePolling(location) == 1) {			
			return;
		}
	}	
		 	
while(1) {
	system("cls");
		printf("\n                       ===ADD NEW DETAILS===          \n");	
	ch = menu(&menuList4[0][0],1,3,18);	
	switch (ch) {
	    case 1:
	        if (temp.noOfVoters == 100) {
	            printf("\nVoter List is full for this constituency! \a");
	        } 	        
			else {	   
	            	            
	            for (i = 0; i < temp.noOfVoters; i++) {
	                if (temp.voterList[i].voterId == 0) {	                    
	                    temp.voterList[i].voterId = temp.id * 1000 + i + 1;	                    	                    
	                    break;
	                }
	            }
	            if(i == temp.noOfVoters) {
	            	temp.voterList[temp.noOfVoters].voterId = temp.id * 1000 + temp.noOfVoters + 1;
				}
	            printf("\nEnter the voter name: ");
	            fflush(stdin);
	            gets(temp.voterList[i].name);	 
	           /* if (i == temp.noOfVoters - 1) {
	                strcpy(temp.voterList[temp.noOfVoters].name, tempName);
	                newId = temp.id * 1000 + temp.noOfVoters + 1;
	                temp.voterList[temp.noOfVoters].voterId = newId;
	            }*/
	            printf("\nVoter ID Generated for the voter: %d", temp.voterList[i].voterId);
	            //printf("\nvoted: %d", temp.voterList[temp.noOfVoters].hasVoted);
	            temp.noOfVoters++;
	            printf("\nVoter added sucessfully...");
	        }
	        break;
	    case 2:
	        if (temp.noOfCandidates == 10) {
	            printf("\nThe candidate list is full \a");
	        }			
			else {
	            printf("\nEnter the new candidate name: ");
	            fflush(stdin);
	            gets(temp.candidateList[temp.noOfCandidates].name);
	            selectCandidateParty(temp.noOfCandidates);
	            temp.candidateList[temp.noOfCandidates].noOfVotes = 0;
	            temp.noOfCandidates++;
	            printf("\nCandidate added sucessfully...");
	        }

	    case 3:
	        break;
	    default:
	        printf("\nYou have entered the wrong choice!");
	    }
	    modifyRecord(&temp,location);
			if(ch==3) {
			break;
		}
		getch();	    
	}	
	
}

//display the voterlist of the constituency
void displayVoterList() {
	system("cls");
	char hasVoted[4],i,j;
	printf("\n                  ===VOTER LIST===      \n");	
	printf("\n%12s  %12s   %12s","Name","VoterID","Has Voted");		
	for(i=0,j=0;j<temp.noOfVoters;i++) {
		if(temp.voterList[i].voterId != 0) {
			j++;
			if(temp.voterList[i].hasVoted == 1) {
				strcpy(hasVoted,"Yes");
			}
			else {
				strcpy(hasVoted,"No");
			}
			printf("\n%12s  %12d  %12s",temp.voterList[i].name,temp.voterList[i].voterId,hasVoted);
		}		
	}		
}

/*Display the candidate list mode = 1 for showing votes (polling result), mode = 0 no votes when voting,
 mode = 2 for displaying for deleting candidate */
void displayCandidateList(int mode) {
int i;								
	for(i=0;i<temp.noOfCandidates;i++) {
		
			if(mode == 1) {
				if (i == 0) {
					printf("\n%12s  %12s  %12s  %12s","Symbol","Name","Party","Votes");
				}
				printf("\n%12c  %12s  %12s  %12d",(char)37 + i,temp.candidateList[i].name,temp.candidateList[i].party.name,temp.candidateList[i].noOfVotes);
			}
			else if(mode == 0) {
				if (i == 0) {
					printf("\n %12s  %12s","Name","Party");
				}
				printf("\n%12c %12s  %12s",(char)37 + i,temp.candidateList[i].name,temp.candidateList[i].party.name);
			}
			else if (mode == 2) {
				if (i == 0) {
					printf("\n%12s  %12s  %12s  %12s","S.No","Name","Party");
				}
				printf("\n%12d  %12s  %12s",i + 1,temp.candidateList[i].name,temp.candidateList[i].party.name);
			}				
	}	
}

//Function to vote
void vote() {
	int found = 1,i,j;
	char symbol;
	printf("\n Enter the Voter ID: ");
	checkInput(&id,MAX,1001);	
	cId = id/1000;
	location = locateRecord(cId);	
	if(location == -1) {
		printf("\nYour voterId is invalid");	
		return;		
	}
	readRecord(location);
	if(temp.stopPolling==1) {
		printf("\nPolling for this constituency has ended!");
		return;	
	}
				
	for(i=0;i<temp.noOfVoters;i++) {
		if(temp.voterList[i].voterId == id) {
			found = 0;
			if(temp.voterList[i].hasVoted==0){
				system("cls");								
				printf("\nYour Name: %s \nYour VoterID: %d",temp.voterList[i].name,temp.voterList[i].voterId);
				printf("\nPress 'V' to vote: ");
				printf("\n\n Yb    dP \n  Yb  dP \n   YbdP \n    YP \n");
				fflush(stdin);
				symbol = getch();
				if(symbol=='V'||symbol=='v') {
					system("cls");
					displayCandidateList(0);
					cId = -1;
					//loop to check if the user has entered the correct symbol or not
										
					for(j = 0; cId<0 || cId>=temp.noOfCandidates;) {
						//if loop has been repeated more than once show below message
						if(j == 1) {
							printf("\nYou have entered wrong choice please enter again : ");
						}
						else{
							printf("\nEnter the SYMBOL of the candidate you want to vote for : ");
						}					 	
						fflush(stdin);
						scanf("%c",&symbol);
						cId = (int) symbol - 37;//'a' = 97 ascii
						j = 1;
					}					
											
					temp.candidateList[cId].noOfVotes += 1;
					temp.voterList[i].hasVoted = 1;	
					modifyRecord(&temp,location);
					printf("\nYou have voted for %s",temp.candidateList[cId].name);
					printf("\nYour vote is registered \a");	
				}
				else {
					return;
				}
						
			} 
			else {			
				printf("\nYou have already voted !");
			}
			break;
		}				
	}			
	if(found) {
		printf("\nYour voterId was not found!");
	}		
}

//find the winner candidate of the current constituency and store the winner candidates party in winnerParty variable
int findWinner() {
	int max = 0,index,i,maxCount=0;
	char resultFileName[26],winningParties[10][50];
	snprintf(resultFileName, 26, "Constituency-Result-%d.txt", temp.id);	
	FILE* result = fopen(resultFileName,"w");
	fprintf(result,"\n %12s  %12s  %12s","Name","Party","Votes");					
	for(i=0;i<temp.noOfCandidates;i++) {			
			fprintf(result,"\n %12s  %12s  %12d",temp.candidateList[i].name,temp.candidateList[i].party.name,temp.candidateList[i].noOfVotes);	
			if(temp.candidateList[i].noOfVotes >= max) {//if two candidates have the same no. of votes then the candidate appearing lower down in the list will be the winner
				max = temp.candidateList[i].noOfVotes;
				index = i;
			}			
		}
		for (i=0;i<temp.noOfCandidates;i++) {
			if(temp.candidateList[i].noOfVotes == max) {
				strcpy(winningParties[maxCount],temp.candidateList[i].party.name);
				maxCount++;				
			}
		}
		if(temp.candidateList[index].noOfVotes == 0) {
			printf("\nNo votes have been registered in this constituency. Minimum 1 vote required to declare winner !");
			return 1;
		}
		else if(maxCount==1){			
			strcpy(temp.winnerParty, temp.candidateList[index].party.name); 		
			printf("\n\n Candidate '%s' in '%s' Party has won in Constituency '%s' with %d Votes!",temp.candidateList[index].name,temp.candidateList[index].party.name, temp.name, temp.candidateList[index].noOfVotes);			
		}
		else {
			strcpy(temp.winnerParty,"");
			printf("\nThe following parties have won the election with equal votes: \n");
			for(i=0;i<=maxCount;i++) {
				puts(winningParties[i]);
			}
		}	
	fclose(result);	
	return 0;
}

//Shows the polling results
void showConstituencyResults() {	
	int pollEnded;	
	if(temp.stopPolling==0) {
		printf("\nPolling has not ended yet, please end the polling first to display the results.");
		pollEnded = togglePolling(location);
	}
	if(temp.stopPolling==1) {
		displayCandidateList(1);		
	}
	modifyRecord(&temp,location);	
					
}

//Delete constituency,voter or candidate
int deleteDetails() {		
	int //for voterid and candidate serial no
	found=1 //for finding the element
	 //last confirmation before deletion
	,cont,//for while
	i; //for determining if poll toggle successfully	
	char menuList5[4][28] = {"Delete Voter","Delete Candidate","Delete Current Constituency","Exit"};
	int noOfConstituencies = noOfRecords();
	//struct Constituency temp2[noOfConstituencies - 1];	
		
	
	//check if the polling is stopped
	if(temp.stopPolling == 1) {
		if(togglePolling(location) == 1) {
			return 0;
		}
	}	
			
	while(1) {
		system("cls");
		printf("\n                       ===DELETE DETAILS===          \n");		
		ch = menu(&menuList5[0][0],1,4,28);					
		switch(ch) {
			case 1:	
					if (temp.noOfVoters == 2) {
	            		printf("\nThe constituency must have atleast 2 voters! \a");	            		 
	            		break;
	        		}			
				printf("\nEnter the Voter Id: ");
						checkInput(&id,temp.id*1000 + 100,temp.id*1000);						
						//we have used this method instead of shift deletion because the generation of voterId 
						//requires that each voter must be present in its respective array index						
						for(i=0;i<temp.noOfVoters;i++) {
								if(temp.voterList[i].voterId==id) {
									found = 0;							
									if(choice()) {
										temp.voterList[i].voterId = 0;
										temp.voterList[i].hasVoted = 0;	
										temp.noOfVoters--;									
										modifyRecord(&temp,location);
										printf("\nVoter deleted sucessfully...");	
									}									
									break;														
								}
						}
						getch();
						printf("i = %d",i);
						if(found==1 && (i == temp.noOfVoters)){
							printf("\nThe record was not found!");
						}																	  
						break;
			case 2:				
				if (temp.noOfCandidates == 2) {
	            	printf("\nThe constituency must have atleast 2 candidates! \a");
	            	break;
	        	}	
				displayCandidateList(2);
				printf("\nEnter the S.No of the candidate you want to delete: ");
				checkInput(&id,temp.noOfCandidates, 1);					
				if(choice()) {
					temp.noOfCandidates--;					
					FILE* PartyList = fopen(fileName,"a");
					checkFile(PartyList);
					//if the candidate to be deleted is from the winning party then update the partyList array
					if(strcmp(temp.winnerParty,temp.candidateList[id - 1].party.name) == 0)	{
						updateResult(-1);
					}									
					fprintf(PartyList,"%s\n",temp.candidateList[id - 1].party.name); //add the party of candidate to be deleted to the partyList
					for(i=id - 1; i<temp.noOfCandidates ; i++) {
						temp.candidateList[i] = temp.candidateList[i + 1];
					}
					fclose(PartyList); //necessary to close the file pointer to update the file as other functions are accessing it
					//temp.candidateList[i] = tempCan;
					printf("\nCandidate deleted sucessfully...");											
				}
				modifyRecord(&temp,location);
														  
				
				break;
			case 3: 	
								/*if(choice() == 1) {	
								tempFile = fopen("temp.dat","w");																
								//Read the first half of the records i.e read no. of records = location										
								fread(temp2,sizeof(struct Constituency),location,Record);
								//Set the file pointer to the end of the record to be deleted
								fseek(Record,sizeof(struct Constituency),SEEK_CUR);
								//Read remaining records to temp2 starting from (temp2 + location)th element
								fread(temp2 + location,sizeof(struct Constituency),noOfConstituencies - location - 1,Record);					
								fwrite(temp2,sizeof(temp),noOfConstituencies - 1,tempFile);								
								rewind(Record);
								fclose(Record);
								fclose(tempFile);
								remove("Records.dat");
								remove(fileName);								
								rename("temp.dat","Records.dat");										
								printf("\nRecord deleted..");
								return 1;																
							}*/
							id = temp.id;
							if(choice() == 1) {
								Record = fopen("Records.dat","rb");
								tempFile = fopen("temp1.txt","w");								
								while(fread(&temp,sizeof(temp),1,Record)) {
									if(temp.id == id) {
										continue;
									}
									fwrite(&temp,sizeof(temp),1,tempFile);
								}
								fclose(Record);
								fclose(tempFile);
								copyFile("temp1.txt","Records.dat");
								remove("temp1.txt");
								remove(fileName);								
								return 1;
							}		
						
			case 4:	break;							
			default:printf("\nYou have entered the wrong choice!"); 		
		}
		if(ch == 4) {
			break;
		}
		getch();				
	}
	return 0;
}

void clearRecords() {
	//notice in this function as well the file Record in not opened hence remove() works
	if(choice() == 1) {
		tempFile = fopen("temp2.dat","w");
		//fclose(Record);
		fclose(tempFile);
		remove("Records.dat");
		rename("temp2.dat","Records.dat");
		printf("\nThe records have been cleared...");	
	}	
}
//int menu(char menuList[][12],int lower, int upper)
int menu(char *menuList,int lower, int upper,int strlen) {
	int position= 0,i,ch;
	char ch2;	
	while(1) {		
		printf("\n\n");
		for(i  = 0;i<upper;i++) {
			if(i==position) {
				printf("-->");
			}
			
			printf("%d. %s\n",i+1,menuList + strlen*i);
		}
		printf("\nUse the ARROW keys to move and press ENTER to select the option or enter the S.No: \n");		
		ch = getch();
		if(ch==13) {
			position++;
			break;
		}		
		else if(ch>=(lower + '0') && ch<=(upper + '0')) {
			position = ch - '0';
			break;
		}
		ch2 = 0;
		if (ch == 0xE0) { // a scroll key was pressed
			ch2 = getch();
		// determine what it was		
			switch(ch2)	{
				//up arrow key
				case 72: 
					if(position == 0) {
						position = upper - 1;
					}
					else { 
						position--;	
					} 
					break;
					//down arrow key
				case 80: 
					if(position == upper - 1) {
						position = 0;
					}
					else { 
						position++;	
					} 
				break;			
		    }
		}
		fflush(stdin);
		system("cls");
	} 
	return position;
}



int main() {
char pswd[51] //for password input from user
,inPass[51],ch2; //for reading pass from file
int n,i;
//FILE* pass = fopen("pass.txt","r");
char menuList1[3][12] = {"Voter Login","Admin Login","Exit"};
char menuList3[8][30] = {"Add Constituency Details","Display VoterList","Polling Results","Delete Constituency Details","Previous Menu","Logout","Start/End Polling","Exit"};
char menuList2[6][30] = {"Enter the constituency ID","Add new Constituency","Declare Election Results","Reset Election Results","Clear All Records","Go Back"};
menu1:
while(1) {
	system("cls");
	printf("\n====  Welcome To Electronic Voter Machine  ====\n\n");
	printf("\n--For Voting select Voter Login\n");
	printf("\n--For modifying Constituencies select Admin Login\n");		
	//printf("%d",&menuList1[0][0]);
//	char *p = &menuList1;
//	printf("\n%s",p + 24);
//	getch();
	switch(menu(&menuList1[0][0],1,3,12)) { 
		case 1: 
			vote();
			break;
		case 2: goto menu2;
			/*printf("\nPlease enter your password (password = 1): ");
			fflush(stdin);
			gets(inPass);
			fgets(pswd,51,pass);
			if(strcmp(inPass,pswd)==0) {
				
			}
			else {
				printf("\n You have entered the wrong password");
			}
			break;*/
		case 3: 
			exit(0);
		default: 
			printf("\nYou have entered the wrong choice!\a");
	}
	printf("\nPress any key to continue.");
	getch();		
}

menu2:
while(1) {
	system("cls");
	initializeResult();
	printf("\n====  Welcome To ADMIN MENU  ====\n\n");
	printf("\n---For modifying existing Constituency, select option 1. \n");
	printf("\n---For creating new Constituency, select option 2. \n");
	printf("\n---For declaring Election Results Party Wise select option 3. \n");
	printf("\n---For resetting Election Results option 4. \n");	
	printf("\n---For deleting all Records select option 5. \n");		
	
	switch(menu(&menuList2[0][0],1,7,30)) {
		case 1:
			printf("\nPlease enter constituency ID (test id = 1): ");			
			checkInput(&cId,MAX,1);
			location = locateRecord(cId);	
			if(location==-1) {
				printf("\nConstituency with this Id does not exist!\a");
			}
			else{
				readRecord(location);							
				initializePartyList();
				goto menu3;				
			}
			break;
		case 2: 
			printf("\nHow many constituencies are needed to be created: ");				
				checkInput(&n,MAX,1);
				//noOfConstituencies += n;
				for(i=0; i<n; i++) {
					printf("\nEnter details for Constituency No. - %d",i+1);
					addConstituencyDetails();
					writeConstituency(&temp);					
					printf("\nConstituency Created Successfully...");
					getch();	
				}
				break;
		case 3:
			declareResult();
			break;
		case 4:
			updateResult(0); //mode = 0 for clearing records.
			break;
		case 5:
			clearRecords();
			break;
		case 6:			
			goto menu1;
		case 7: 
			showAll();
			break;
		default: 
			printf("\nYou have entered the wrong choice!\a");			
	}
	printf("\nPress any key to continue.");
	getch();
}

menu3:
while(1) {
	system("cls");	
	switch(menu(&menuList3[0][0],1,8,30)) {					
		case 1: 
			updateConstituencyDetails();
			break;
		case 2: 
			displayVoterList();			  		
			break;
		case 3: 
			showConstituencyResults();				  		
			break;
		case 4: 			
			if (deleteDetails() == 1) {				
				goto menu2;
			}			
			break;			
		case 5: 			
			goto menu2;			
		case 6: 					
			goto menu1;
		case 7: 					
			togglePolling(location);
			break;												 	  
		case 8: 
			exit(0);	
		default:  
			printf("\nYou have entered the wrong choice!");
	} //switch end
	printf("\nPress any key to continue.");
	getch();
}
system("cls");
return 0;
}//Main end
