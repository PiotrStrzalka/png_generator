# png_generator
One of interview code challenges

# Zadanie 1


Zadanie polega na napisaniu 2 programów, w dalszej treści nazywanych odpowiednio program
1 i program 2. Program 1 jest źródłem komend opisujących obraz, program 2 jest
odpowiedzialny za przetwarzanie otrzymanych od programu 1 komend i przygotowanie na ich
bazie obrazu. Obraz ten ten ma być następnie zapisany do pliku.
Programy powinny być przygotowane wg poniższych wymagań:
- ***REQ1 Język programowania - C/C++***
- ***Programy muszą kompilować się na Linuksie a wszelkie niestandardowe biblioteki powinny być załączone z rozwiązaniem. //REQ2***
- ***REQ3 Program 1 uruchamia program 2***
- ***Program 1 umożliwia wprowadzanie poleceń:***
    - ***z klawiatury, zatwierdzanych naciśnięciem ENTER, //REQ4***
    - ***z pliku jako parametru podanego przy uruchamianiu programu 1 //REQ5***
- ***Program 1 przesyła polecenia do programu 2 w dowolny sposób z wyłączeniem gniazd (socketów),  //REQ6***
- ***Program 2 odbiera i analizuje polecenia. Jeżeli polecenie zostanie  rozpoznane poprawnie program 2 je realizuje, w przeciwnym wypadku odsyła do programu 1 komunikat błędzie. //REQ7***
- ***REQ8 Program 1 zapisuje do pliku log.txt niepoprawne komendy otrzymane od programu 2.***
- ***Programy realizują następujący minimalny zestaw komend:***
    - ***SET_WIDTH W - komenda określa szerokość ekranu w pikselach, W jest liczbą całkowitą określającą ilość pikseli.  //REQ9***
    - ***SET_HEIGHT H - komenda określa wysokość ekranu w pikselach, H jest liczbą całkowitą określającą ilość pikseli.  //REQ10***
    - ***DRAW_RECTANGLE X,Y,W,H - komenda służy do rysowania prostokąta, którego lewy górny róg ma współrzędne X,Y a W i H oznaczają odpowiednio szerokość i wysokość prostokąta. //REQ11***
    - ***DRAW_TRIANGLE X1,Y1,X2,Y2,X3,Y3 - komenda służy do rysowania trójkąta którego rogi mają współrzędne poziome i pionowe odpowiednio w punktach (X1,Y1), (X2,Y2) oraz (X3,Y3).  //REQ12***
    - ***RENDER NAME - przygotowuje obraz oraz zapisuje go do pliku w wybranym formacie graficznym (BMP, PNG, JPEG) pod nazwą określoną parametrem “NAME”.  //REQ13***

Wymagane artefakty:
- źródła obu programów,
- plik testowy z komendami,
- skrypt budujący make lub cmake,