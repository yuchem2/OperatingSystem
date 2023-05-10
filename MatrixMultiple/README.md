
# Matrix Multiplication using multiple thread

## Objectives

확장자가 없는 파일 형태로 저장된 행렬을 받는다. (.\x64\Debug에 예시 파일들이 존재한다.)   
입력된 행렬들의 곱을 멀티프로세싱을 통해 연산한다. 이때 각 행렬들은 행렬곱이 가능하게 입력된다고 가정한다.  
CUI로는 행렬 연산의 차원과 계산 시간을 출력한다. 이때 각 행렬곱 연산은 최대 1초가 소요되어야 한다.  


## Flow Chart

### Divide Problem
![image](https://github.com/yuchem2/OperatingSystem/assets/101711808/35e00411-85fe-41cf-85dc-e894b0f6e61b)

### Main Process
![image](https://github.com/yuchem2/OperatingSystem/assets/101711808/56634c6e-354e-4733-8ae5-f02a6c36b5b9)

### Threads
![image](https://github.com/yuchem2/OperatingSystem/assets/101711808/1023ab65-91bf-4d2d-925f-c62c830054ee)


## Execute

인자 없이 Windows Command Processor에서 다음 명령어를 입력하면 된다. SavedDircetory는 현재 MultipleMatrix 프로젝트가 저장된 폴더 명이다. 
```
SavedDirectory\MultipleMatrix\x64\Debug\MultipleMatrix.exe 3x4 4x5 5x6 6x10
```

## Test Example 

<img src="https://github.com/yuchem2/OperatingSystem/assets/101711808/860c0593-6727-4678-b232-41e50012b297" width=700>
<img src="https://github.com/yuchem2/OperatingSystem/assets/101711808/6f25b9d7-9f56-4986-9a84-b1782fb57274" width=700>
