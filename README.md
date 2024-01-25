# Projekt C++

## Opis Projektu
Jest to prosta gra 2D typu hack and slash stworzona w stylu pixel art. Gracz ma do dyspozycji postać, którą może poruszać się po ekranie. W trakcie rozgrywki tworzą się wrogowie, którzy zmierzają w kierunku gracza, a po wejściu z nim w kontakt zadają mu obrażenia. Gracz ma do dyspozycji broń, która zadaje obrażenia przeciwnikom. Po pokonaniu przeciwnika gracz otrzymuje punkty. Wraz ze wzrostem wyniku przeciwnicy tworzą się coraz szybciej. Celem rozgrywki jest przetrwanie jak najdłużej ilości czasu i zdobycie jak największego wyniku. 

Gra oferuje:
   - 2 klasy postaci do wyboru
   - 2 rodzje broni
   - 2 rodzaje wrogów
   - 3 wzmocnienia 


## Wymagania Systemowe i potrzebne biblioteki
Przed uruchomieniem projektu, upewnij się, że spełniasz następujące wymagania:
- System operacyjny: Windows 10 lub 11
- Program Microsoft Visual 2022 z zainstalowanym językiem C++ 

Biblioteki SDL2, które zostały użyte do tworzenia programu znajdują się w repozytorium i zostały podłączone do projektu dynamicznie 


## Jak Zainstalować:
**Wyłącznie rozgrywka:**
   - pobierz plik Installer.msi
   - urochom go
   - wybierz ścieżkę do instalacji gry
   - po skończonej instalacji uruchom skrót znajdujący się na pulpicie lub plik .exe w miejscu instalacji gry

**Rozgrywka, edycja i kompilacja gry:**
   - Sposób 1 - w teminalu wpisz komende: ```"git clone --depth 1 https://github.com/IgnacyZywalewski/ProjektCpp"```. Następnie otwórz projekt w programie Microsoft Visual
   - Sposób 2 - w programie Microsoft Visual sklonuj repozytorium. Program powinien być gotowy do urochomienia. Jeśli z jakiegoś powodu istnieje problem z bibliotekami oto instrukcja ich instalacji:
      - w programie Microsoft Visual klknij w zakładkę Projekt i wybierz właściwości
      - wybierz zakładkę Katalogi VC++:
         - w opcji Katalogi plików nagłówkowych dodaj ścieżkę do folderu SDL2\include (ścieżka może się róznić w zależności gdzie znajduje się folder z biblioteką)
         - w opcji Katalogi biblotek dodaj ścieżkę do folderu SDL2\lib\x64
      - te same kroki wykonaj dla biblioteki SDL2_image i SDL2_ttf (upewnij się że podajesz odpowiednie ścieżki do każdego z folderów)
      - wybierz zakładkę Konsolidator -> Dane wejściowe :
         - w opcji Dodatkowe zależności wpisz ręcznie lub skopiuj: 'SDL2.lib; SDL2main.lib; SDL2_image.lib; SDL2_ttf.lib;'
      - wciśnij przycisk ok
      - upewnij się że w folderze, w którym znajdują się pliki źródłowe znajdują się pliki: SDL2.dll, SDL2_image.dll i SDL2_ttf.dll
      - po zainstalowanu bibliotek nie zmieniaj ich położenia, a jeśli to zrobisz pamiętaj o zmianie ich ścieżek we właściwościach projektu
   - skompiluj i uruchom projekt


## Sterowanie
- **Działania przycisków:**
   - Aby aktywować działania przycisków (np. Start, Exit, Replay, ...) należy w nie klknąć

- **Ruch:**
   - w lewo: strzałka w lewo
   - w prawo: strzałka w prawo
   - w górę: strzałka w górę
   - w dół: strzałka w dół

- **Pauza rozgrywki:** 
    - przcisk 'P' na klawiaturze
    - kliknięcie w ikone pauzy na ekranie

- **Wyjście z gry:**:
    - w trakcie rozgrywki należy kliknąć przycik ESC
    - na ekranie startowym i końcowym należy klkinąć przycisk Exit 