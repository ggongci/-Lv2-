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


		//방 그리기를 기분 나빠짐 코드보다 먼저 써야할지.. 아니면 기분 나빠짐 코드가 우선인지.. 문제로다.
}