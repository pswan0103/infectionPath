 //
//  ifs_element.c
//  InfestPath
//
//  Created by Juyeop Kim on 2020/10/20.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ifct_element.h"

typedef enum place {
    Seoul,          //0
    Jeju,           //1
    Tokyo,          //2
    LosAngeles,     //3
    NewYork,        //4
    Texas,          //5
    Toronto,        //6
    Paris,          //7
    Nice,           //8
    Rome,           //9
    Milan,          //10
    London,         //11
    Manchester,     //12
    Basel,          //13
    Luzern,         //14
    Munich,         //15
    Frankfurt,      //16
    Berlin,         //17
    Barcelona,      //18
    Madrid,         //19
    Amsterdam,      //20
    Stockholm,      //21
    Oslo,           //22
    Hanoi,          //23
    Bangkok,        //24
    KualaLumpur,    //25
    Singapore,      //26
    Sydney,         //27
    SaoPaulo,       //28
    Cairo,          //29
    Beijing,        //30
    Nairobi,        //31
    Cancun,         //32
    BuenosAires,    //33
    Reykjavik,      //34
    Glasgow,        //35
    Warsow,         //36
    Istanbul,       //37
    Dubai,          //38
    CapeTown        //39
} place_t;

char countryName[N_PLACE+1][MAX_PLACENAME] =
{   "Seoul",
    "Jeju",
    "Tokyo",
    "LosAngeles",
    "NewYork",
    "Texas",
    "Toronto",
    "Paris",
    "Nice",
    "Rome",
    "Milan",
    "London",
    "Manchester",
    "Basel",
    "Luzern",
    "Munich",
    "Frankfurt",
    "Berlin",
    "Barcelona",
    "Madrid",
    "Amsterdam",
    "Stockholm",
    "Oslo",
    "Hanoi",
    "Bangkok",
    "KualaLumpur",
    "Singapore",
    "Sydney",
    "SaoPaulo",
    "Cairo",
    "Beijing",
    "Nairobi",
    "Cancun",
    "BuenosAires",
    "Reykjavik",
    "Glasgow",
    "Warsow",
    "Istanbul",
    "Dubai",
    "CapeTown",
    "Unrecognized"
};

char* ifctele_getPlaceName(int placeIndex)
{
	return countryName[placeIndex];
}

typedef struct ifcs_ele {
	int pIndex;			//번호: 정수
	int age;			//나이: 정수 
	int time;			//감염 시점: 정수 
	place_t placeHist[N_HISTORY];	//감염 직전 이동 경로: enum 배열
} ifcs_ele_t;


void* ifctele_genElement(int index, int age, unsigned int detected_time, int history_place[N_HISTORY])
{ 
	ifcs_ele_t* ptr;
	
	ptr = (ifcs_ele_t*)malloc(5*sizeof(ifcs_ele_t*)); 
	
	ptr->pIndex = index;
	ptr->age = age;
	ptr->time = detected_time;
	ptr->placeHist[N_HISTORY] = history_place[N_HISTORY];	//ptr이 장소 경로 가리킴 

	return ptr;
}


int ifctele_getHistPlaceIndex(void* obj, int index) {
	ifcs_ele_t *ptr = (ifcs_ele_t *)obj;
	return (ptr->placeHist[index]);
}

unsigned int ifctele_getinfestedTime(void* obj) {
	ifcs_ele_t *ptr = (ifcs_ele_t *)obj;
	return (ptr->time);
}

int ifctele_getAge(void* obj) {
	ifcs_ele_t *ptr = (ifcs_ele_t *)obj;
	return (ptr->age); 
}

void ifctele_printElement(void* obj) {
	ifcs_ele_t *ptr = (ifcs_ele_t *)obj;

	//print elements
	printf("index: %i\n", ptr->pIndex);
	printf("Age: %i\n", ptr->age); 
    printf("time: %i\n", ptr->time);
    printf("History: ");
    
//    int i;
//    for(i=0; i<N_HISTORY; i++)
//		ptr->placeHist[i] = ifctele_getplaceName(ptr->placeHist[i]);
//	
//	for(i=0; i<N_HISTORY; i++)
//		printf("%s ", ptr->placeHist[i]);
		
}


unsigned int ifctele_getinfestedTime(void* obj);


