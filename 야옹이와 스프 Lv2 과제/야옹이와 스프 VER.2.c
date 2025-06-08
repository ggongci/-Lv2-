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
	int mouse = 0;
	int laser = 0;
	int close = -1;  //현재 위치에서 더 가까운 쪽 놀이기구를 골라서 이동하기 위해 그 위치를 target 변수에 저장
	int feel_so_bad_or_good = 6 - intimacy;
	//미니게임을 위한 변수
	int turn_count = 1;
	int number = 1;       //지금 부를 숫자 1~30까지
	int pnp = 1;  //(pass no pass) 패논패  
	int 십의자리 = number / 10;
	int 일의자리 = number % 10;

	printf("**** 쫀득이와 수프 레벨 투 ****\n");

	printf("              ／＞　 フ\n");
	printf("              |  _　_ | \n");
	printf("           ／ ミ＿xノ \n");
	printf("          /　　　　 |\n");
	printf("         /　 ＼　　 /\n");
	printf("        │　　|　|　|\n");
	printf("  ／￣  |　　 |　|　|\n");
	printf("      (￣＼＿_＼_)__)       (츄르)\n");
	printf("  ＼二)       \n");

	printf("당신이 지은 야옹이 이름을 다시 한 번 입력하세요: ");  //%c는 문자열을 하나만 다룰 때 사용하고
	scanf_s("%s", name, 100);                 //%s 는 여러 글자 문자열을 입출력 할 때 사용.  계속해서 한자로 오류난 이유가 %c로 돼 있어서 그랬음 
	printf("정답입니다. 당신의 야옹이의 이름은 %s입니다.\n", name);

	Sleep(500);
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

		Sleep(500);

		// 돌발 퀘스트: 3,6,9 게임 (3턴마다 실행)
		if (turn_count % 3 == 0) {
			char input[10];  //"짝" 을 입력해야하니까 사용자가 입력한 문자열을 저장할 배열! 
			number = 1;
			pnp = 1;
			printf("\n[돌발 퀘스트 발생!] %s과(와) 함께 3,6,9 게임을 시작합니다!\n", name);
			printf("규칙: 3, 6, 9가 들어간 숫자에는 '짝'이라고 입력하세요!\n");
			printf("30까지 성공하면 돌발 퀘스트는 성공입니다!\n");
			printf("성공의 보상은 %s(이)의 꾹꾹이 입니다.", name);
			Sleep(1000);

			while (number <= 30) {   //30까지니까 <= 30
				if (number % 2 == 1) {      //홀수 차례 = 고양이 라는 의미 
					if (number % 3 == 0 || number % 10 == 3 || number % 10 == 6 || number % 10 == 9)
						// %3 == 0 : 3의 배수인지 // %10 == 3/6/9 : 끝자리가 3, 6 ,9인지 
						// 이 조건 중에 하나라도 맞으면 "짝" 출력, 아니면 숫자 출력!	
						printf("고양이: 짝\n");
					else
						printf("고양이: %d\n", number);
					Sleep(500);
				}
				else {
					printf("당신의 차례! >> ");
					scanf_s("%s", input, 10);
					//int need_clap 은 "짝" 을 외쳐야하는 숫자인지 저장하는 변수
					십의자리 = number / 10;
					일의자리 = number % 10;

					int need_clap = (십의자리 == 3 || 십의자리 == 6 || 십의자리 == 9 ||
						일의자리 == 3 || 일의자리 == 6 || 일의자리 == 9);

					if (need_clap) {
						//"짝"이 아닌 경우 직접 비교하는 코드

						//아래와 같은 코드로 돌발 퀘스트를하려고 했지만 한글 한 글자는 2바이트 이상이여서 1바이트만 처리하는 char 는 오류가 발생할 수 밖에 없습니다.
						//그로인해 strcmp 라는 함수를 사용해서 이 부분을 해결하고자 합니다. 
						//strcmp는 두개의 문자열이 같은지, 다른지 다르면 어떤식으로 다른지 검사할 수 있는 strcmp 함수입니다. 
						//현재 코드에서 사용하는 최적화된 코드
					//	if (input[0] != '짝' || input[1] != '\0') {   //input[0] 은 사용자가 입력한 첫번째 글자임 
						if (strcmp(input, "짝") != 0) {
							//짝은 고양이와 약속한 단어 //'\0' 이건 문자열의 끝을 의미하는 특수 문자 
									//3 6 9 게임을 만들기 위해 찾아보면서 알게 됨
									//위 조건은 입력한 값이 "짝" 한 글자가 아니면 틀림다는 걸 의미함 
							printf("틀렸습니다! 숫자 %d에서는 '짝'이라고 했어야 합니다!\n", number);
							printf("집사 자격 감점 1점!");
							pnp = 0;
							break;
						}
					}
					else {
						if (number < 10) {
							if (!(input[0] == number + '0' && input[1] == '\0')) {
								//이건 숫자 한 자리일 때
								printf("아이고.. 이걸 틀려..? 숫자 %d을 정확히 입력해야 했어요!\n", number);
								Sleep(400);
								printf("\n이거마저 해내지못하면 당신은 집사 탈락이야.");
								Sleep(200);
								printf("\n. . . \n조만간 기회를 다시 줄테니 연습하도록");
								pnp = 0;
								break;
							}
						}
						else {
							if (number > 10) {
								if (!(input[0] == number / 10 + '0' && input[1] == number % 10 + '0' && input[2] == '\0')) {
									//이건 숫자가 두 자리일 때 
									printf("틀렸습미다! 숫자 %d 을 정확히 입력해야 했어요\n", number);
									pnp = 0;
									Sleep(400);
									break;
								}
							}
						}
					}
				}
				number++;
			}

			if (pnp == 1 && number > 30) {
				printf("\n축하합니다! 3,6,9 게임에 성공했습니다!\n");
				feel++;
				if (feel > 3) feel = 3;
				cp += 100;
				Sleep(200);
				printf("기분 +1, CP +100 획득! 현재 기분: %d, CP: %d\n", feel, cp);
			}
			else {
				printf("\n아쉽네요! 3,6,9 게임 실패... \n");
				if (feel > 0) feel--;
				cp = 0;
				printf("기분이 나빠졌습니다. 현재 기분: %d, 보유 CP는 모두 압수냥 (CP: %d)\n", feel, cp);
			}
			Sleep(1500);
		}
		//이후 turn_count는 루프 마지막에서 ++ 해줘야 함
		turn_count++;
		//상태출력 이후에는 -> 기분 나빠짐.
		//기분 나빠짐
		feel_so_bad_or_good = 6 - intimacy; //여기서 매번 다시 계산 아니면 계속 4로 나와버림 
		dice = rand() % 6 + 1;
		printf("주사위의 눈이 %d이하이면 그냥 기분이 나빠집니다. (6 - %d)\n", feel_so_bad_or_good, intimacy);  //이 부분은 새로운 변수를 하나 만들 예정!
		printf("주사위를 굴립니다. 또르르...\n");
		Sleep(500);
		printf("% d이(가) 나왔습니다.\n", dice);
		Sleep(500);
		if (dice <= (feel_so_bad_or_good)) {
			if (feel > 0) {
				feel--;
				printf("%s의 기분이 나빠집니다: %d -> %d\n", name, feel + 1, feel);
				Sleep(500);
			}
			else {
				printf("%s의 기분은 이미 최악입니다.\n", name);
				Sleep(500);
			}
		}
		else {
			printf("다행히 기분이 유지됩니다.\n");
			Sleep(500);
		}

		Sleep(1000);
		//현재 코드까지 실행했을 때는 기분 나빠짐 부분만 반복해서 나오는 중 - 수정이 필요할듯

		//이동 & 행동 
		// 이동
		//기분이 0 일 때 집쪽으로이동
		zzontteok_prev_pos = zzontteok_pos;  //이전 위치를 저장

		if (feel == 0) {
			if (zzontteok_pos > HME_POS) {
				zzontteok_pos--;
				printf("기분이 매우 나쁜 %s은(는) 집으로 향합니다.\n", name);
				Sleep(500);
				printf("기분은 나쁘지만 다리가 짧아서 한 칸 밖에 이동하지 못합니다.\n");
				Sleep(500);
				if (zzontteok_pos == HME_POS) kicked_off_shoes = 1;
			}  //현재 위 코드는 집에 막 도착한 턴에도 바로 기분 + 1 을 하는 문제점이 있음. //! 플래그 ! 변수 하나를 추가해서 해결했음
			else {
				printf("%s은(는) 집에서 쉼니다.\n", name);
				Sleep(500);
				if (kicked_off_shoes) {
					printf("%s은(는) 이번 턴에 막 집에 도착했습니다. 아직 쉬지 않습니다.\n", name);
					kicked_off_shoes = 0; // 다음 턴부터는 쉴 수 있도록 플래그 변수 초기화
				}
				else if (feel < 3) {
					printf("기분이 조금 나아졌습니다: %d -> %d\n", feel, feel + 1);
					feel++;         //기분이 0이면 무조건 집으로 감. 도착 후엔 기분+1 회복!
				}
				Sleep(500);
			}
		}

		//기분이 1 일 때 더 가까운 놀이기구 쪽으로 이동, 놀이기구가 없으면 기분 -1
		else if (feel == 1) {
			if (scratcher || tower) {
				close = -1;
				if (scratcher && (!tower || abs(zzontteok_pos - scratcher_pos) <= abs(zzontteok_pos - tower_pos)))
					close = scratcher_pos;
				else {
					close = tower_pos;
				}
				if (zzontteok_prev_pos < close) zzontteok_pos++;
				else if (zzontteok_pos > close) zzontteok_pos--;


				printf("%s은(는) 심심해서 %s 쪽으로 한 칸 이동합니다.\n", name,

					close == scratcher_pos ? "스크래처" : "캣타워");
				Sleep(500);
			}
			else {
				printf("놀거리가 없어서 %s의 기분이 매우 나빠집니다.\n", name);
				if (feel > 0) feel--;
				Sleep(500);
			}
		}
		//기분이 2 일 때 제자리에 대기
		else if (feel == 2) {
			printf("\n%s은(는) 기분 좋게 식빵을 굽고 있습니다.\n", name);
			Sleep(500);

		}
		//기분 3 일 때 냄비쪽으로 이동
		else if (feel == 3) {
			if (zzontteok_pos < BWL_POS) {
				zzontteok_pos++;
				printf("%s은(는) 골골송을 부르며 수프를 만들러 한 칸 이동합니데이.\n", name);
				Sleep(500);
			}
			else {
				printf("%s은(는) 수프를 만들 준비가 되어 있습니다~!\n", name);
				Sleep(500);
			}
		}		//냄비 쪽으로 무조건 이동 → 도착 시 수프 만들기 가능

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
				Sleep(500);
			}
		}
		else if (zzontteok_pos == tower_pos && tower) {
			if (feel < 3) {
				int before = feel;
				feel += 2;
				if (feel > 3) feel = 3;
				printf("%s은(는) 캣타워를 뛰어다닙니다.\n", name);
				printf("기분이 제법 좋아졌습니다: %d -> %d\n", before, feel);
				Sleep(500);
				//스크래처와는 다르게 캣타워는 기분이 2일 때 feel += 2 → 4가 되는데 
				//하지만 기분은 최대 3까지니까 feel = 3; 으로 아에 못 박은 것 
				//그리고 스크래처처럼 feel + 2 를 하면 최대 3까진데 4가 나와버림 그러면 feel = 3으로 만들어야하고 이렇게 출력하면 
				//실제로는 2->3인데 3->5라고 나와버림. (feel값이 바뀌어버리기때문에 feel + 2는 원래 값에 + 2가 안되기 때문에??)
				//before라는 변수를 만들어서 바뀌기 전 값을 저장해서 기분을 +2 하면 feel은 알아서 최대값을 3으로 해주게 됨
			}
		}
		Sleep(1500);

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

		//현재 코드를 실행해도 상호작용 입력에서 장난감을 사지 않았을 경우의 코드까지 나오고 있음
		//mouse 와 laser 가 매턴 1로 초기화되도록 해서 게임 시작하자마자 장난감 있는 것처럼 나오고, 실제로는 안 샀어도 쓸 수 있는 오류 생긴것.
		//그러므로 1 을 0으로 수정 그리고 위치 또한 while문 밖으로 빼줘야함! 
		//int mouse = 1;
		//int laser = 1;
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
			printf("\n아무것도 하지 않았습니다.\n");
			Sleep(600);
			break;
		case 1:
			printf("\n%s(이)의 턱을 긁어 주었습니다.\n", name);
			Sleep(600);
			break;
		case 2:
			if (mouse && laser) {
				printf("장난감 쥐와 레이저 포인터 중 하나를 선택하세요.\n");
			}
			else if (mouse) {
				printf("\n장난감 쥐로 야옹이와 놀아 주었습니다.\n");
				Sleep(600);
			}
			else if (laser) {
				printf("\n레이저 포인터로 야옹이와 놀아 주었습니다.\n");
				Sleep(600);
				break;
			}
		case 3:
			printf("\n레이저 포인터로 야옹이와 놀아 주었습니다.\n");
			Sleep(600);
			break;
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
			printf("%s(이)의 기분은 그대로입니다: %d\n", name, feel);
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

		// 2-7 CP 생산
		int cp_gain = (feel - 1 >= 0 ? feel - 1 : 0) + intimacy;
		cp += cp_gain;

		printf("\n%s의 기분(0~3): %d\n", name, feel);
		printf("집사와의 친밀도(0~4): %d\n", intimacy);
		printf("%s의 기분과 친밀도에 따라서 CP가 %d 포인트 생산되었습니다.\n", name, cp_gain);
		printf("보유 CP: %d 포인트\n", cp);


		int shop_flex = -1; //사용자가 상점에서 선택한 번호를 저장할 변수 (-1 로 저장해서 초기값은 아무것도 안 골랐다는 뜻)

		printf("\n[ 고양이 상점 ]\n");
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
		Sleep(1500);
		system("cls");

	}
	return 0;
}