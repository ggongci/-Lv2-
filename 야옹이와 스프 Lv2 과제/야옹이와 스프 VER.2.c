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
	int kicked_off_shoes = 0; //집에 막 도착했는지 표시해주는 코드 (0: 도착 X, 1: 이번 턴에 막 도착)
	//상태를 표현하기 위한 목적은“플래그 변수" 라고 부름.

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
		//현재 코드까지 실행했을 때는 기분 나빠짐 부분만 반복해서 나오는 중 - 수정이 필요할듯

		//이동 & 행동 
		// 이동
		//기분이 0 일 때 집쪽으로이동
		zzontteok_prev_pos = zzontteok_pos;
		if (feel == 0) {
			if (zzontteok_pos != HME_POS) {
				printf("기분이 매우 나쁜 %s은(는) 집으로 향합니다.\n", name);
				zzontteok_pos = HME_POS;    //이 코드가 집으로 이동하게 하는 코드!
				kicked_off_shoes = 1;
			}  //현재 위 코드는 집에 막 도착한 턴에도 바로 기분 + 1 을 하는 문제점이 있음. //! 플래그 ! 변수 하나를 추가해서 해결했음
			else {
				printf("%s은(는) 집에서 쉼니다.\n", name);
				if (kicked_off_shoes == 1) {
					printf("%s은(는) 이번 턴에 막 집에 도착했습니다. 아직 쉬지 않습니다.\n", name);
					kicked_off_shoes = 0; // 다음 턴부터는 쉴 수 있도록 플래그 변수 초기화
				}
				else if (feel < 3) {
					printf("기분이 조금 나아졌습니다: %d -> %d\n", feel, feel + 1);
					feel++;         //기분이 0이면 무조건 집으로 감. 도착 후엔 기분+1 회복!
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

		//행동
		//아래 코드는 야옹이 Lv1에서 사용한 코드
		//수프 만드는 코드
		if (zzontteok_pos == BWL_POS) {
			int soup_type = rand() % 3;
			if (soup_type == 0) {
				printf("%s이(가) 감자 수프를 만들었습니다!\n", name);
			}
			else if (soup_type == 1) {
				printf("%s이(가) 양송이 수프를 만들었습니다!\n", name);
			}
			else {
				printf("%s이(가) 브로콜리 수프를 만들었습니다!\n", name);

			}
			madesoup++;     //씀으로써 몇 개를 만들었는지 확인할 수있게 해주는 코드.


			//일단 흐름상... 수프를 만든 직후에 CP가 생성되는 게 좋을 거 같아서 넣음. 
			//기분이 좋으면 수프를 만들러 가니까 수프를 만들면 CP 포인트를 생성 
			//cute point 보다는 cooking point 느낌. . . . .  ^^7 
			
			//2-7 CP 생산 

			int cp_get = feel + intimacy; //CP 계산식: 기분 + 친밀도
			cp += cp_get; //현재 턴에 얻은 CP를 누적

			printf("\n%s의 기분(0~3): %d\n", name, feel);
			printf("집사와의 친밀도(0~4): %d\n", intimacy);
			printf("%s의 기분과 친밀도에 따라서 CP가 %d 포인트 생산되었습니다.\n", name, cp_get);
			printf("보유 CP: %d 포인트\n", cp);
		}
		//위 코드를 배열로 간단히 변경할 수도 있음! 하지만 스스로 아래와 같은 배열을 사용하기엔 부족함을 느낌.
		/*	if (zzontteok_pos == BWL_POS) {
				int soup_type = rand() % 3;
				const char* soup_names[] = { "감자 수프", "양송이 수프", "브로콜리 수프" };
				printf("%s이(가) %s를 만들었습니다!\n", name, soup_names[soup_type]);
				madesoup++;
			} */
			//스크래처 긁기 코드 O 
		else if (zzontteok_pos == scratcher_pos && scratcher) {
			if (feel < 3) {
				printf("%s은(는) 스크래처를 긁고 놀았습니다.\n", name);
				printf("기분이 조금 좋아졌습니다: %d -> %d\n", feel, feel + 1);    //스크래처는 3을 넘을 일이 없으니까 feel 에 +1 만 해줘도 됨
				feel++;           //집에서 한 턴을 쉴 때마다 기분 + 1
			}
		}
		else if (zzontteok_pos == tower_pos && tower) {
			if (feel < 3) {
				int before = feel;
				feel += 2;
				if (feel > 3) feel = 3;
				printf("%s은(는) 캣타워를 뛰어다닙니다.\n", name);
				printf("기분이 제법 좋아졌습니다: %d -> %d\n", before, feel);
				//스크래처와는 다르게 캣타워는 기분이 2일 때 feel += 2 → 4가 되는데 
				//하지만 기분은 최대 3까지니까 feel = 3; 으로 아에 못 박은 것 
				//그리고 스크래처처럼 feel + 2 를 하면 최대 3까진데 4가 나와버림 그러면 feel = 3으로 만들어야하고 이렇게 출력하면 
				//실제로는 2->3인데 3->5라고 나와버림. (feel값이 바뀌어버리기때문에 feel + 2는 원래 값에 + 2가 안되기 때문에??)
				//before라는 변수를 만들어서 바뀌기 전 값을 저장해서 기분을 +2 하면 feel은 알아서 최대값을 3으로 해주게 됨
			}
		}
		Sleep(1500);
		system("cls");

		//방 그리기 

		printf("\n[ %s(이)가 보면 울부짖을 정도의 방 . . . ]\n", name);
		for (int i = 0; i < ROOM_WIDTH + 2; i++) printf("#");
		printf("\n#");
		for (int i = 0; i < ROOM_WIDTH; i++) {
			if (i == HME_POS) printf("H");
			else if (i == tower_pos && tower) printf("T");
			else if (i == scratcher_pos && scratcher) printf("S");
			else if (i == BWL_POS) printf("B");
			else printf(" ");
		}
		printf("#\n#");
		for (int i = 0; i < ROOM_WIDTH; i++) {
			if (i == zzontteok_pos) printf("C");
			else if (i == zzontteok_prev_pos) printf(".");
			else printf(" ");
		}
		printf("#\n");
		for (int i = 0; i < ROOM_WIDTH + 2; i++) printf("#");
		printf("\n\n");

		Sleep(1500);
		system("cls");

		int mouse = 1;
		int laser = 1;
		int toy_choice = 1; //무조건적을 필요한 변수! 
		//장난감이 없을 때 toy_choice 값이 1 : 이 얘기는 선택 가능한 게 0, 1
		//그럼 장난감이 하나 일 때는 toy_choice 값이 2 : 이 얘기는 선택 가능한 게 0, 1, 2
		//장난감이 둘 다 있을 때는 toy_choice 값이 3 : 이 얘기는 선택 가능한 게 0, 1, 2, 3

		//상호작용 입력
		//장난감이 없을 경우
		printf("\n어떤 상호작용을 하시겠습니까?\n");
		printf("0. 아무것도 하지 않음\n");
		printf("1. 긁어 주기\n");

		//장난감이 2개 다 있을 경우
		if (mouse && laser) {
			printf("2. 장난감 쥐로 놀아 주기\n");
			printf("3. 레이저 포인터로 놀아 주기\n");
			toy_choice = 3;
		}
		else if (mouse) {
			printf("2. 장난감 쥐로 놀아 주기\n");    //쥐만 있는 경우
			toy_choice = 2;
		}
		else if (laser) {
			printf("2. 레이저 포인터로 놀아 주기\n");  //레이저만 있는 경우
			toy_choice = 2;
		}

		//범위 안의 숫자가 나올 때까지 다시 입력하기 위한 do while 문
		do {
			printf(">> ");
			scanf_s("%d", &interaction);
			if (interaction < 0 || interaction > toy_choice) {
				printf("잘못된 번호입니다. 다시 입력하세요.\n");
			}
		} while (interaction < 0 || interaction > toy_choice);

		//선택 출력만! (효과는 다음 단계에서) 
		switch (interaction) {
		case 0:
			printf("아무것도 하지 않았습니다.\n");
			break;
		case 1:
			printf("야옹이를 긁어 주었습니다.\n");
			break;
		case 2:
			if (mouse && laser) {
				printf("장난감 쥐와 레이저 포인터 중 하나를 선택하세요.\n");
			}
			else if (mouse) {
				printf("장난감 쥐로 야옹이와 놀아 주었습니다.\n");
			}
			else if (laser) {
				printf("레이저 포인터로 야옹이와 놀아 주었습니다.\n"); break;
			}
		case 3:
			printf("레이저 포인터로 야옹이와 놀아 주었습니다.\n"); break;
		}

		//상호작용 처리에서 주사위가 몇 이상일 때 이하일 때의 코드들도 존재해야함.. 이 부분을 놓쳤으면 큰일날뻔
		dice = rand() % 6 + 1;
		//2-6 상호작용 처리
		switch (interaction) {
		case 0: // 아무것도 하지 않았을 때 
			if (feel > 0) {  //기분이 0 미만으로 내려가지 않게 하기 위해 feel > 0 으로 둠
				printf("%s의 기분이 나빠졌습니다: %d -> %d\n", name, feel, feel - 1);
				feel--;
			}
			else {
				printf("%s의 기분은 이미 최악입니다. 당장 놀아주지 않으면 할퀴기 공격이 들어올 수도 있습니다.\n", name);
			}
			if (dice <= 5 && intimacy > 0) {   //이 부분도 위 부분과 같이 친밀도가 음수가 되지 않게 하기 위해!
				printf("주사위: %d → 집사와의 관계가 나빠집니다: %d -> %d\n", dice, intimacy, intimacy - 1);
				intimacy--;
			}
			else {
				printf("주사위: %d → 관계는 그대로입니다.\n", dice);
			}
			break;

		case 1: // 긁어주기
			printf("%s의 기분은 그대로입니다: %d\n", name, feel);
			if (dice >= 5) {   //주사위가 5 이상이면 +1 이니까 
				printf("주사위: %d → 집사와의 관계가 좋아집니다: %d -> %d\n", dice, intimacy, intimacy + 1);
				intimacy++;
			}
			else {        //주사위가 5 이하 일 때는 그대로 가버리는것
				printf("주사위: %d → 관계는 그대로입니다.\n", dice);
			}
			break;

		case 2: // 장난감 쥐
			if (feel < 3) {    //기분은 3보다 클 수 없으므로 위 코드들과 같이 3 초과를 막기 위해 
				printf("장난감 쥐로 %s와 놀아주었습니다. %s의 기분이 쬐끔 좋아졌습니다: %d -> %d\n", name, name, feel, feel + 1);
				feel++;
			}
			else {
				printf("기분은 이미 최고입니다.\n");
			}
			if (dice >= 4) {        //이번엔 주사위가 4 이상 일 때 
				printf("주사위: %d → 집사와의 관계가 좋아집니다: %d -> %d\n", dice, intimacy, intimacy + 1);
				intimacy++;
			}
			else {
				printf("주사위: %d → 관계는 그대로입니다.\n", dice);
			}
			break;

		case 3: // 레이저 포인터
			if (feel < 3) {
				int before = feel;  //before 변수를 사용한 이유는 행동 코드때와 같음. 
				//feel 값이 바뀌어도 나중에 값을 저장했으니까 기억이 가능 안전함.
				feel += 2;
				if (feel > 3) feel = 3;    //위에 말 했을 최대 기분은 3 이니까 feel = 3 을 넣어서 제한 시켜버리기
				printf("레이저 포인터로 %s와 신나게 놀아주었습니다. %s의 기분이 꽤 좋아졌습니다: %d -> %d\n", name, name, before, feel);
			}
			else {
				printf("기분은 이미 최고입니다.\n");
			}
			if (dice >= 2) {
				printf("주사위: %d → 집사와의 관계가 더욱 돈독해집니다: %d -> %d\n", dice, intimacy, intimacy + 1);
				intimacy++;
			}
			else {
				printf("주사위: %d → 관계는 그대로입니다.\n", dice);
			}
			break;
		}

		int shop_flex = -1; //사용자가 상점에서 선택한 번호를 저장할 변수 (-1 로 저장해서 초기값은 아무것도 안 골랐다는 뜻)

		printf("\n[ 고양이 상점 🐾 ]\n");
		printf("\n고양이 상점에서 물건을 살 수 있습니다.\n");
		printf("어떤 물건을 구매할까요?\n");
		printf("0. 아무 것도 사지 않는다.\n");  //0번은 그냥 지나가는 선택지로.

		//이미 구매한 물건은 (품절) 표시를 해야함 먼저. 
		if (!mouse) {      //mouse 가 0이면 참이고, 즉 아직 장난감 쥐를 안 샀는지 확인하는 조건문 !mouse 는 mouse == 0과 같음 
			               //가독성 향상을 위해 제가 직접 mouse == 0 를 !mouse 바꿔서 사용크하하
			printf("1. 장난감 쥐: 1 CP\n");
		}
		else {
			printf("1. 장난감 쥐: 1 CP (품절)\n");
		}
		//레이저 포인터는 2 CP로 설정
		if (!laser) {
			printf("2. 레이저 포인터: 2 CP\n");
		}
		else {
			printf("2. 레이저 포인터: 2 CP (품절)\n");
		}
		//스크래처와 캣 타워는 각각 4 CP, 6 CP로 설정
		if (!scratcher) {
			printf("3. 스크래처: 4 CP\n");
		}
		else {
			printf("3. 스크래처: 4 CP (품절)\n");
		}

		if (!tower) {
			printf("4. 캣 타워: 6 CP\n");
		}
		else {
			printf("4. 캣 타워: 6 CP (품절)\n");
		}

		do {                 //범위를 벗어난 값이 입력되면 다시 입력 받아야하므로.
			printf(">> ");	 //do while 문을 사용한 이유는 무조건 한번은 입력 받고, 틀렸으면 다시 입력받는건 반복하기 위해서임
			scanf_s("%d", &shop_flex);
			if (shop_flex < 0 || shop_flex > 4) {
				printf("잘못된 입력입니다. 다시 입력하세요.\n");
			}
		} while (shop_flex < 0 || shop_flex > 4); 

		switch (shop_flex) {
		case 0:
			printf("아무 것도 사지 않았습니다.\n");
			break;

		case 1:
			if (mouse) {
				printf("장난감 쥐는 이미 구매했습니다.\n");  //이미 구매한 물건
			}
			else if (cp >= 1) {
				cp -= 1;				  //CP가 충분하면 CP 차감
				mouse = 1;				  //mouse를 1로 바꿔서 "장난감 쥐 보유 중" 표시
				printf("장난감 쥐를 구매했습니다. 보유 CP: %d 포인트\n", cp);
			}
			else {
				printf("CP가 부족합니다. 현재 CP: %d 포인트\n", cp);   //CP 부족하면 구매 실패 메시지
			}  break;

		case 2:
			if (laser) {
				printf("레이저 포인터는 이미 구매했습니다.\n");
			}
			else if (cp >= 2) {
				cp -= 2;
				laser = 1;
				printf("레이저 포인터를 구매했습니다. 보유 CP: %d 포인트\n", cp);
			}
			else {
				printf("CP가 부족합니다. 현재 CP: %d 포인트\n", cp);
			}
			break;

		case 3:
			if (scratcher) {
				printf("스크래처는 이미 구매했습니다.\n");
			}
			else if (cp >= 4) {
				cp -= 4;
				scratcher = 1;

				//방 안의 랜덤한 위치에 스크래처를 놓아야하는데 기존 가구와 다른 위치에 배치해야함으로 || 사용 
				///이미 있는 장소(집, 냄비, 캣타워)에는 놓지 않도록 반복 체크
				do {
					scratcher_pos = rand() % ROOM_WIDTH;     //무작위로 위치를 생성해야하니까 주사위와 같은 코드 사용! 
					//만약 스크래처가 집, 냄비, 캣 타워 위치 중 하나랑 같다면 다시 뽑는다. 
					//|| 사용한 이유!
				} while (scratcher_pos == HME_POS || scratcher_pos == BWL_POS || scratcher_pos == tower_pos);
				printf("스크래처를 구매했습니다. 보유 CP: %d 포인트\n", cp);
			}
			else {
				printf("CP가 부족합니다. 현재 CP: %d 포인트\n", cp);
			}
			break;

		case 4:
			if (tower) {
				printf("캣 타워는 이미 구매했습니다.\n");
			}
			else if (cp >= 6) {
				cp -= 6;
				tower = 1;

				//스크래처와 같이 방 안의 랜덤한 위치에 캣 타워를 놓아야함. 
				//중복되지 않는 위치이지만 무작위로 위치하는 코드다.
				do {
					tower_pos = rand() % ROOM_WIDTH;
				} while (tower_pos == HME_POS || tower_pos == BWL_POS || tower_pos == scratcher_pos);
				printf("캣 타워를 구매했습니다. 보유 CP: %d 포인트\n", cp);
			}
			else {
				printf("CP가 부족합니다. 현재 CP: %d 포인트\n", cp);
			}
			break;
		}
	}
	return 0;
}