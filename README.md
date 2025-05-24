# 📌 DoFlow - 팀 할 일 관리 툴

> C++ 백엔드 + 웹 프론트엔드로 구현된 협업용 Todo 관리 시스템

---

## 🔧 기술 스택

- **C++20**, [Drogon](https://github.com/drogonframework/drogon) Framework
- HTML + JavaScript (Vanilla)
- JSON 파일 기반 데이터 저장
- Linux (WSL2 or Ubuntu), CMake

---

## 🖼️ 주요 기능

- ✅ 할 일 CRUD (생성/수정/삭제)
- ✅ 완료 상태 체크 및 진행률 시각화
- ✅ 마감일 설정 및 지연 시 시각 경고
- ✅ 카테고리별 필터링
- ✅ 상세 설명 작성
- ✅ 완료 숨기기, 검색 필터
- ✅ 전체 초기화 버튼
- ✅ JSON 파일을 통한 **데이터 영속성**

---

## 🗂️ 프로젝트 구조

```
DoFlow/
├── controllers/
│   ├── TodoController.h/.cc
│   ├── TodoStorage.h/.cc
├── models/
│   └── Todo.h
├── public/
│   └── index.html
├── todos.json         <-- 자동 생성되는 저장 파일
├── CMakeLists.txt
├── main.cc
```

---

## 🚀 실행 방법

```bash
# 1. 빌드
cd build
cmake ..
make

# 2. 실행
./DoFlow

# 3. 브라우저 접속
http://localhost:8800
```

---

## 📸 스크린샷

> 원하는 경우 진행률 바 / 필터 / 설명 등 UI 캡처 이미지 추가

---

## 📦 설치 필요

- Drogan
- jsoncpp
- CMake
- g++ (C++20 지원)
- WSL2 or Ubuntu

---

## 📜 라이선스

MIT License
