# SBM-U 2.0 ODF++ 저비용 정화조 모니터링 시스템

Arduino 기반의 정화조 모니터링 시스템으로, 초음파 거리 센서, 탁도 센서, TDS 센서, 그리고 온도 센서를 이용해 정화조 상태를 실시간으로 측정

## 기능

- 초음파 거리 센서로 정화조 채움률 측정
- 탁도 센서로 물의 혼탁도 점수 계산
- TDS 센서로 전기전도도(raw 값) 및 점수 계산
- Dallas 온도 센서로 수온 측정
- 측정값을 종합해 `수거 필요도` 점수를 산출
- 수거 필요도를 4단계(긴급, 빠른 수거, 정기 수거, 정상)로 출력

## 사용된 센서 및 핀

- `trigPin` (D2): 초음파 송신
- `echoPin` (D3): 초음파 수신
- `turbidityPin` (A0): 탁도 센서 입력
- `ONE_WIRE_BUS` (A1): Dallas 온도 센서 OneWire 버스
- `tdsPin` (A2): TDS 센서 입력

## 동작 방식

1. 초음파 센서를 통해 물 표면까지 거리 측정
2. 탱크 깊이(`TANK_DEPTH`)를 기준으로 채움률 계산
3. 아날로그 탁도 값을 `TURB_CLEAN`, `TURB_DIRTY` 기준으로 0~100 점수로 변환
4. 아날로그 TDS 값을 `TDS_CLEAN`, `TDS_DIRTY` 기준으로 0~100 점수로 변환
5. 온도를 DallasTemperature 라이브러리로 읽기
6. 가중치를 적용해 최종 `수거 필요도` 점수 산출
   - 채움률 60%
   - 탁도 25%
   - TDS 15%
7. 점수에 따라 운영 상태 메시지 출력

## 점수 기준

- `>= 80`: 🔴 긴급 수거
- `60 ~ 79.9`: 🟠 빠른 수거 필요
- `40 ~ 59.9`: 🟡 정기 수거 대상
- `< 40`: 🟢 정상

## 설치 및 실행

1. Arduino 보드를 연결
2. `OneWire.h` 및 `DallasTemperature.h` 라이브러리를 설치
3. `stemun.ino`를 Arduino IDE에서 열기
4. 코드 업로드 후 시리얼 모니터(`9600`bps)로 데이터 확인

## HTML UI 연결

- `index.html` 파일을 열어 웹 UI로 데이터를 확인할 수 있습니다.
- Web Serial API를 이용해 Arduino의 시리얼 데이터를 브라우저에서 직접 읽습니다.
- Chrome/Edge 계열 브라우저에서 `localhost` 또는 `HTTPS` 환경에서 실행해야 합니다.
- VS Code의 Live Server 확장이나 간단한 로컬 서버를 사용하면 편리합니다.

### 실행 방법 예시

1. VS Code에서 `index.html` 열기
2. Live Server로 실행하거나, 터미널에서 `python3 -m http.server 8000` 실행
3. 브라우저에서 `http://localhost:8000` 접속
4. `시리얼 연결` 버튼을 눌러 Arduino 포트를 선택

## 참고

- 수온 센서 읽기 오류 시 `-127.0°C` 값을 반환하며, 이 경우 `오류`로 표시됩니다.
- `TANK_DEPTH`, `TURB_CLEAN`, `TURB_DIRTY`, `TDS_CLEAN`, `TDS_DIRTY` 값을 실제 센서 특성에 맞춰 보정하면 더 정확한 모니터링이 가능합니다.
