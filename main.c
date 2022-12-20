//
//  main.c
//  infestPath
//
//  Created by Juyeop Kim on 2022/10/20.
//

#include <stdio.h>
#include <string.h>
#include "ifct_element.h"
#include "ifct_database.h"

#define MENU_PATIENT        1
#define MENU_PLACE          2
#define MENU_AGE            3
#define MENU_TRACK          4
#define MENU_EXIT           0

#define TIME_HIDE           2


int trackInfester(int patient_no, int *detected_time, int *place);
int isMet(int patient_no, int met_patient_no); 
int convertTimeToIndex(int time, int infestedTime);

int main(int argc, const char * argv[]) {
    
    int menu_selection;
    void *ifct_element;
    FILE* fp;
    int pIndex, age, time;
    int placeHist[N_HISTORY];
    
    //------------- 1. loading patient info file ------------------------------
    //1-1. FILE pointer open
    if (argc != 2)
    {
        printf("[ERROR] syntax : infestPath (file path).");
        return -1;
    }
    
    fp = fopen(argv[1],"r");
    if (fp == NULL)
    {
        printf("[ERROR] Failed to open database file!! (%s)\n", argv[1]);
        return -1;
    }
    fclose(fp);
    
    
    //1-2. loading each patient informations
    
    fp = fopen("patientInfo_prb2.txt", "r");
    int i;
    
    while (3 == fscanf(fp, "%i %i %i", &pIndex, &age, &time))
    {
    	for(i=0; i<5; i++)
 			fscanf(fp, "%i", &placeHist[i]);
 			
		ifct_element = ifctele_genElement(pIndex, age, time, placeHist);
 		ifctdb_addTail(ifct_element);
	}
	
	
    //1-3. FILE pointer close
    fclose(fp);
    
	  
    do {
        printf("\n\n=============== INFECTION PATH MANAGEMENT PROGRAM (No. of patients : %i) =============\n", ifctdb_len());
        printf("1. Print details about a patient.\n");                      //MENU_PATIENT
        printf("2. Print list of patients infected at a place.\n");        //MENU_PLACE
        printf("3. Print list of patients in a range of age.\n");          //MENU_AGE
        printf("4. Track the root of the infection\n");                     //MENU_TRACK
        printf("0. Exit.\n");                                               //MENU_EXIT
        printf("=============== ------------------------------------------------------- =============\n\n");
        
        printf("Select a menu :");
        scanf("%d", &menu_selection);
        fflush(stdin);
        
        int sel;										//1번, 4번 메뉴 환자 번호 
        int maxAge, minAge;								//3번 메뉴 나이 최대 최소 
        char sel_place[MAX_PLACENAME];					//2번 메뉴 장소 선택 
        
        
        switch(menu_selection)
        {
            case MENU_EXIT:
                printf("Exiting the program... Bye bye.\n");
                break;
                
            case MENU_PATIENT:
            	//환자 번호 입력받고 정보 출력 
				 
            	printf("Select a patient index(0~%i): ", ifctdb_len()-1);
				scanf("%i", &sel);
				printf("\n");
					
				if (sel > (ifctdb_len()-1) || sel < 0) {		//환자 번호 입력 오류시 
					printf("[ERROR] Wrong index selection! (%i), please choose between 0 ~ %i\n", sel, ifctdb_len()-1);
				}
				 
				else {
					ifct_element = ifctdb_getData(sel);
					printf("\n%ith patient's information\n\n", sel);
					ifctele_printElement(ifct_element);
				}
				
                break;
                
            case MENU_PLACE:
            	
                printf("Enter the place of infection: "); 
                scanf("%s", &sel_place);
				
				int i, j;
                for(i=0; i<ifctdb_len(); i++) {
                	ifct_element = ifctdb_getData(i);
                	
                	for (j=0; j<N_HISTORY; j++) {
                		int placeIndex[N_HISTORY];
                		placeIndex[j] = ifctele_getHistPlaceIndex(ifct_element, j);
                		
                		if (strcmp(ifctele_getPlaceName(placeIndex[j]), sel_place) == 0) {
                			printf("\n\n");
                			ifctele_printElement(ifct_element);
              			}
                	}
				}
				
                break;
                
            case MENU_AGE:
            	
            	printf("Enter the range of patient age(min~max): "); 
            	scanf("%i~%i", &minAge, &maxAge);
            
            	for (i=0; i<ifctdb_len(); i++) {
            		ifct_element = ifctdb_getData(i);
            		
            		if (minAge <= ifctele_getAge(ifct_element) && ifctele_getAge(ifct_element) <= maxAge) {
            			printf("\n\n");
            			ifctele_printElement(ifct_element);
            		}
				}

                break;
                
            case MENU_TRACK:
            	printf("Select a patient to track(0~%i): ", ifctdb_len()-1);
            	scanf("%i", &sel);
            	printf("\n");
				
				ifct_element = ifctdb_getData(sel);
				
				if (sel > (ifctdb_len()-1) || sel < 0) {
					printf("[ERROR] Wrong index selection! (%i), please choose between 0 ~ %i\n", sel, ifctdb_len()-1);
				}
				
                else 
				{
                    int patient = sel;
                    int contg_patient;
                    
					while (patient >= 0)
					{
						int detectedTime = ifctele_getinfestedTime(ifct_element);
						int detectedPlace = ifctele_getHistPlaceIndex(ifct_element, N_HISTORY);
						contg_patient = trackInfester(patient, detectedTime, detectedPlace);
						
						if (contg_patient >= 0)
							printf("%i 환자는 %i 환자에게 전파됨\n", patient, contg_patient);
						
						else
							contg_patient = patient;
					}
				}
                
                break;
                
            default:
                printf("[ERROR Wrong menu selection! (%i), please choose between 0 ~ 4\n", menu_selection);
                break;
        }
    
    } while(menu_selection != 0);
    
    return 0;
}

int trackInfester(int patient_no, int *detected_time, int *place)
{
	int cog_patient_no;
	int j;
	for(j=0; j<ifctdb_len(); j++) 
	{
		int met_patient_no[ifctdb_len()];
		int mettime[ifctdb_len()];
		
		mettime[j] = isMet(patient_no, met_patient_no[j]);
		
		if (mettime[j] > 0)
		{
			if (mettime[j] < mettime[j-1])
			{
				cog_patient_no = j;
			}
		}
	}
	
	return cog_patient_no;
}

int isMet(int patient_no, int met_patient_no)
{
	void* ifct_element;
	ifct_element = ifctdb_getData(patient_no);
	
	int infestedtime = ifctele_getinfestedTime(ifct_element);	//감염 확인 시간 
	int placetime[3];											//i번째 이동장소 시점
	int placeIndex[3];										//i번째 이동장소 인덱스 
	int mettime;
	
	int j;
	for (j=2; j<N_HISTORY; j++)
	{
		placetime[j] = infestedtime - N_HISTORY + j;
		placeIndex[j] = convertTimeToIndex(placetime[j], infestedtime);
		
		void* ifct_element_met;
		ifct_element_met = ifctdb_getData(met_patient_no);
		
		if (ifctele_getHistPlaceIndex(ifct_element, placeIndex[j]) == ifctele_getHistPlaceIndex(ifct_element_met, placeIndex[j]))
			mettime = placetime[j];
	}
	 
	return mettime;
}

int convertTimeToIndex(int time, int infestedTime)
{
	int index = -1;
	
	if (time <= infestedTime && time > infestedTime-N_HISTORY)
	{
		index = N_HISTORY-(infestedTime - time)-1;
	}
	
	return index;
}

