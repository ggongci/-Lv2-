#include <stdio.h>
#include <Windows.h> //Sleep(millis)–밀리초만큼 실행을 잠시 멈추게 하기 위한 함수 
#include <stdlib.h> //system(cmd)–프로그램실행중, 명령프롬프트에서 실행 가능한 명령어cmd를 실행하는 함수: srand(seed) 함수
#include <time.h> //랜덤값이 나오기 위해 무조건 써야함!
#define ROOM_WIDTH 15 //Lv1과 다르게 방의 크기를 15로 설정함.
#define HME_POS 1 //집의 위치를 나타냄
#define BWL_POS (ROOM_WIDTH - 2) //냄비의 위치를 나타냄

//ver. 2 에서 한턴–상태출력–기분나빠짐-이동& 행동–방그리기–상호작용및결과–CP 생산–상점구매

int main(void) {
	srand((unsigned int)time(NULL)); //실행할 때마다 다른 랜덤 값을 만들기 위해 사용

	char name[100];
	int madesoup = 0; //스프를 만들었는지 여부를 나타내는 변수
	int intimacy = 2; //친밀도 2로 시작
	int feel = 3; //기분 3으로 시작
	int cp = 0; //cp(cute point). 초기값 0, 매 턴 (MAX(0, 기분-1) + 친밀도) 만큼의 CP를 얻음
	int interaction = -1;  //상호작용 선택의 값
	int dice = 0;
	int zzontteok_pos = HME_POS;         // 현재 쫀떡이 위치
	int zzontteok_prev_pos = -1;         // 직전 위치 
	int scratcher_pos = -1;           //스크래처 위치
	int tower_pos = -1;               //캣 타워 위치          [게임 시작 시 설치되지 않은 상태이므로 -1 초기화를 하는 것!
	int scratcher = 0, tower = 0; //해당 놀이기구를 샀는지 여부 (0 or 1)

	printf("당신이 지은 야옹이 이름을 다시 한 번 입력하세요: ");  //%c는 문자열을 하나만 다룰 때 사용하고
	scanf_s("%s", name, 100);                 //%s 는 여러 글자 문자열을 입출력 할 때 사용.  계속해서 한자로 오류난 이유가 %c로 돼 있어서 그랬음 
	printf("정답입니다. 당신의 야옹이의 이름은 %s입니다.\n", name);

	Sleep(1000);
	system("cls");  //화면을 지움

	//2-1상태창 변경 - CP , 기분 출력
	while (1) {
		// 상태 출력
		printf("==================== 현재 상태 ====================\n");
		printf("현재까지 만든 수프: %d개\n", madesoup);
		printf("CP: %d 포인트\n", cp);

		printf("%s 기분(0~3): %d\n", name, feel);
		switch (feel) {
		case 0: printf("기분이매우나쁩니다.\n"); break;
		case 1: printf("심심해합니다.\n"); break;
		case 2: printf("식빵을굽습니다.\n"); break;
		case 3: printf("골골송을부릅니다.\n"); break;
		}

		printf("집사와의 관계 (0~4): %d\n", intimacy);
		switch (intimacy) {
		case 0: printf("곁에 오는 것조차 싫어합니다.\n"); break;
		case 1: printf("간식 자판기 취급입니다.\n"); break;
		case 2: printf("그럭저럭 쓸 만한 집사입니다.\n"); break;
		case 3: printf("훌륭한 집사로 인정 받고 있습니다.\n"); break;
		default: printf("집사 껌딱지입니다.\n"); break;
		}
		printf("==================================================\n\n");


		//상태출력 이후에는 -> 기분 나빠짐.
		//기분 나빠짐
		dice = rand() % 6 + 1;
		printf("6 - %d: 주사위의 눈이 6-%d이하이면 그냥 기분이 나빠집니다.\n", intimacy, intimacy);  //이 부분은 새로운 변수를 하나 만들 예정!
		printf("주사위를 굴립니다. 또르르...\n%d이(가) 나왔습니다.\n", dice);
		if (dice <= (6 - intimacy)) {
			if (feel > 0) {
				feel--;
				printf("%s의 기분이 나빠집니다: %d -> %d\n", name, feel + 1, feel);
			}
			else {
				printf("%s의 기분은 이미 최악입니다.\n", name);
			}
		}
		else {
			printf("다행히 기분이 유지됩니다.\n");
		}

		Sleep(1000);
	}   //현재 코드까지 실행했을 때는 기분 나빠짐 부분만 반복해서 나오는 중 - 수정이 필요할듯

	//이동 & 행동 
	//기분이 0 일 때 집쪽으로이동
	if (feel == 0) {
		if (zzontteok_pos != HME_POS) {
			printf("기분이 매우 나쁜 %s은(는) 집으로 향합니다.\n", name);
			zzontteok_pos = HME_POS;    //이 코드가 집으로 이동하게 하는 코드!
		}
		else {
			printf("%s은(는) 집에서 쉼니다.\n", name);
			if (feel < 3) {
				feel++;         //기분이 0이면 무조건 집으로 감. 도착 후엔 기분+1 회복!
				printf("기분이 조금 나아졌습니다: %d -> %d\n", feel - 1, feel);
			}
		}
	}
	//기분이 1 일 때 더 가까운 놀이기구 쪽으로 이동
	else if (feel == 1) {
		if (scratcher && zzontteok_pos != scratcher_pos) {
			printf("%s은(는) 심심해서 스크래처 쪽으로 이동합니다.\n", name);
			zzontteok_pos = scratcher_pos;          //이 코드가 스크래처로 이동하게 하는 코드!
		}
		else if (tower && zzontteok_pos != tower_pos) {
			printf("%s은(는) 심심해서 캣타워 쪽으로 이동합니다\n", name);
			zzontteok_pos = tower_pos;             //이 코드가 타워로 이동하게 하는 코드!
		}
		else {
			printf("놀 거리가 없어서 %s의 기분이 매우 나빠집니다.\n", name);
			if (feel > 0) feel--;                //스크래처 또는 캣타워가 있으면 그 쪽으로 이동. 없으면 기분-1!
		}
	}
	//기분이 2 일 때 제자리에 대기
	else if (feel == 2) {
		printf("%s은(는) 기분 좋게 식빵을 굽고 있습니다.\n", name);
		// 이동하지 않고 대기 상태. 텍스트만 출력되고 이동 없음
	}
	//기분 3 일 때 냄비쪽으로 이동
	else if (feel == 3) {
		if (zzontteok_pos != BWL_POS) {
			printf("%s은(는) 골골송을 부르며 수프를 만들러 갑니다.\n", name);
			zzontteok_pos = BWL_POS;            //이 코드가 냄비로 이동하게 하는 코드!
			//냄비 쪽으로 무조건 이동 → 도착 시 수프 만들기 가능
		}
	}

