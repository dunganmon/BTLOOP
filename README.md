# BTLOOP
#build: g++ (Get-ChildItem src/*.cpp) -Iheader -o library

(-Iheader: Nó bảo g++: "Nếu một file .cpp nói #include "Menu.h", mày hãy vào thư mục header/ để tìm nó".)
((Get-ChildItem src/*.cpp) Nó sẽ lấy danh sách tất cả các file .cpp trong thư mục src và "nhét" danh sách đó vào vị trí của g++, giúp g++ nhận đúng tất cả các file.)

#run: .\library.exe
