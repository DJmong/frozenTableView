# frozenTableView
I made a frozen Column Table

>사용 언어
- C++11
- Qt4

>레퍼런스
- https://doc.qt.io/qt-6/qtwidgets-itemviews-frozencolumn-example.html


>구조 설명
![frozen](https://user-images.githubusercontent.com/49215689/229468626-16a7e78d-7e20-4166-87c2-f767f629d7a8.png)
- 동일한 Model 을 공유하는 TableView 2개를 생성
- 고정할 Column 값 만큼을 TableView 1 에서 출력
- 나머지 Column 값을 TableView 2 에서 출력
- TableView 1 의 HorizontalScrollBar 비활성화
- TableView 2 의 HorizontalScrollBar 활성화
- TableView 2 ScrollBar만 사용할 수 있으므로 TableView 1의 Column은 변하지 않음

![frozenColumn](https://user-images.githubusercontent.com/49215689/229465986-acf67010-df9f-4c17-b598-f052cb0f0f94.gif)

>만든 이유
- 위 사진과 같이 Column 고정 기능이 필요하다고 해서 만듦

![image](https://user-images.githubusercontent.com/49215689/229467098-e372061b-caa2-4a0a-bd45-879de4715939.png)

>사용법
- QTableView 사용하는 것과 같이 사용
- setLastFrozenColumn(int) 함수를 통해 고정할 Column 값 입력
