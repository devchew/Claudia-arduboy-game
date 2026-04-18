# Balance Upgrades Tool

Narzędzie do analizy balansu upgrade'ów vs capacity serwerów.
Includuje bezpośrednio `gameLogic.h` i `State.h` — jedno źródło prawdy, bez duplikacji parametrów.

## Wymagania

- Kompilator C++17 (g++ / clang++)
- make

## Uruchomienie

```bash
cd tools
make run
```

Wynik zapisany do pliku:

```bash
make && ./balance_upgrades > out.txt
```

## Co pokazuje

- Parametry sparsowane z plików gry
- Capacity na różnych etapach gry
- Koszty i bonusy leveli serwerów
- Koszty każdego upgrade'u (level po levelu)
- Podsumowanie kosztów wszystkich upgrade'ów
- Symulacja inboundu po każdym zakupie z progress barem vs max capacity
- Wykrywanie punktu overflow (inbound > capacity)
