/*
 * Narzędzie do analizy balansu upgrade'ów vs capacity serwerów.
 * Includuje bezpośrednio gameLogic.h / State.h — jedno źródło prawdy.
 *
 * Kompilacja i uruchomienie:
 *   cd tools && make
 *   ./balance_upgrades
 */

#include <cstdio>
#include <cstdint>
#include <cmath>
#include <cstring>

// PROGMEM musi być zdefiniowany przed State.h (który go używa a nie includuje Arduboy2.h)
#define PROGMEM

// === INCLUDE PLIKÓW GRY (jedno źródło prawdy) ===
#include "../Sketch/State.h"
#include "../Sketch/gameLogic.h"

// === HELPER: formatowanie liczb z separatorami tysięcy ===
void printNum(char* buf, size_t sz, int64_t n) {
    if (n < 0) { buf[0] = '-'; printNum(buf+1, sz-1, -n); return; }
    if (n < 1000) { snprintf(buf, sz, "%lld", (long long)n); return; }
    char tmp[32];
    printNum(tmp, sizeof(tmp), n / 1000);
    snprintf(buf, sz, "%s,%03lld", tmp, (long long)(n % 1000));
}

const char* fmt(int64_t n) {
    static char bufs[4][32];
    static int idx = 0;
    idx = (idx + 1) % 4;
    printNum(bufs[idx], sizeof(bufs[idx]), n);
    return bufs[idx];
}

void printBar(double pct) {
    int barLen = (int)(pct / 5);
    if (barLen > 20) barLen = 20;
    printf("[");
    for (int i = 0; i < 20; i++) printf("%c", i < barLen ? '#' : '.');
    printf("]");
}

void printSep() {
    for (int i = 0; i < 100; i++) printf("=");
    printf("\n");
}

// === MAIN ===

int main() {
    const int maxSlots = MaxRacks * RackSize;
    const uint32_t maxCap = maxSlots * getServerUpgradeBonus(MaxServerLevel);

    // --- PARAMETRY ---
    printf("PARAMETRY Z PLIKÓW GRY:\n");
    printf("  MaxRacks=%d, RackSize=%d, MaxServerLevel=%d\n", MaxRacks, RackSize, MaxServerLevel);
    printf("  serverPrice=%d, serverLevelCapacityScale=%d\n", serverPrice, serverLevelCapacityScale);
    printf("  Upgrades: %d\n", MaxUpgrades);
    for (int i = 0; i < MaxUpgrades; i++) {
        printf("    %d: %-25s price=%10s  bonus=%5d  max=%d\n",
            i, (const char*)upgradesNames[i],
            fmt(upgrades[i].startingPrice), upgrades[i].startingBonus, upgrades[i].max);
    }

    printf("\nMAX CAPACITY = %d slots x server_bonus(%d)=%s = %s\n",
        maxSlots, MaxServerLevel, fmt(getServerUpgradeBonus(MaxServerLevel)), fmt(maxCap));

    // --- CAPACITY NA RÓŻNYCH ETAPACH ---
    printf("\n"); printSep();
    printf("  CAPACITY na roznych etapach gry\n");
    printSep();

    struct { const char* label; uint32_t cap; } milestones[] = {
        {"Start (1 srv lvl 1)",          1 * getServerUpgradeBonus(1)},
        {"1 rack filled (4 srv lvl 1)",  4 * getServerUpgradeBonus(1)},
        {"1 rack maxed (4 srv lvl 20)",  4 * getServerUpgradeBonus(MaxServerLevel)},
        {"3 racks filled lvl 5",        12 * getServerUpgradeBonus(5)},
        {"3 racks filled lvl 10",       12 * getServerUpgradeBonus(10)},
        {"3 racks filled lvl 15",       12 * getServerUpgradeBonus(15)},
        {"ALL MAXED",                    maxCap},
    };
    for (auto& m : milestones) {
        printf("  %-35s => capacity = %6s\n", m.label, fmt(m.cap));
    }

    // --- SERVER LEVELS ---
    printf("\n"); printSep();
    printf("  SERVER LEVELS (1 serwer, lvl 1-20)\n");
    printSep();
    printf("  %3s | %10s | %6s | %14s | %14s | %10s\n",
        "Lvl", "Capacity", "Gain", "Upgrade cost", "Cumul cost", "Cost/cap");
    uint32_t cumulCost = 0;
    uint32_t prevCap = 0;
    for (int lvl = 1; lvl <= MaxServerLevel; lvl++) {
        uint32_t cap = getServerUpgradeBonus(lvl);
        uint32_t gain = cap - prevCap;
        uint32_t cost = getServerUpgradeCost(lvl);
        cumulCost += cost;
        double costPerCap = cap > 0 ? (double)cumulCost / cap : 0;
        printf("  %3d | %10s | +%4s | %14s | %14s | %10.1f\n",
            lvl, fmt(cap), fmt(gain), fmt(cost), fmt(cumulCost), costPerCap);
        prevCap = cap;
    }

    // --- KOSZTY UPGRADE'ÓW ---
    printf("\n"); printSep();
    printf("  KOSZTY UPGRADE'OW (price * 1.5^level)\n");
    printSep();
    for (int i = 0; i < MaxUpgrades; i++) {
        Upgrade& u = upgrades[i];
        printf("\n  %s (base price: %s, max lvl: %d)\n",
            (const char*)upgradesNames[i], fmt(u.startingPrice), u.max);
        printf("  %3s | %14s | %14s | %10s | %10s\n",
            "Lvl", "Koszt lvl", "Koszt cumul", "Bonus", "Cost/bonus");
        uint32_t cumCost = 0;
        for (int lvl = 1; lvl <= u.max; lvl++) {
            Upgrade tmp = {u.startingPrice, u.startingBonus, (uint8_t)lvl, u.max};
            uint32_t cost = getOfficeUpgradeCost(Upgrade{u.startingPrice, u.startingBonus, (uint8_t)lvl, u.max});
            cumCost += cost;
            uint32_t bonus = getOfficeUpgradeBonus(tmp);
            double cpb = bonus > 0 ? (double)cumCost / bonus : 0;
            printf("  %3d | %14s | %14s | %10s | %10.1f\n",
                lvl, fmt(cost), fmt(cumCost), fmt(bonus), cpb);
        }
    }

    // Podsumowanie kosztów
    printf("\n  %2s | %-25s | %14s | %14s | %14s\n",
        "#", "Upgrade", "Koszt 1 lvl", "Koszt max lvl", "Koszt total");
    uint64_t grandTotal = 0;
    for (int i = 0; i < MaxUpgrades; i++) {
        Upgrade& u = upgrades[i];
        uint32_t costFirst = getOfficeUpgradeCost(Upgrade{u.startingPrice, u.startingBonus, 1, u.max});
        uint32_t costLast = getOfficeUpgradeCost(Upgrade{u.startingPrice, u.startingBonus, u.max, u.max});
        uint64_t total = 0;
        for (int lvl = 1; lvl <= u.max; lvl++) {
            total += getOfficeUpgradeCost(Upgrade{u.startingPrice, u.startingBonus, (uint8_t)lvl, u.max});
        }
        grandTotal += total;
        printf("  %2d | %-25s | %14s | %14s | %14s\n",
            i, (const char*)upgradesNames[i], fmt(costFirst), fmt(costLast), fmt((int64_t)total));
    }
    printf("  %2s | %-25s | %14s | %14s | %14s\n",
        "", "GRAND TOTAL", "", "", fmt((int64_t)grandTotal));

    // --- ANALIZA: OBECNE WARTOŚCI ---
    printf("\n"); printSep();
    printf("  OBECNE WARTOSCI (compound 50%%)\n");
    printSep();

    // Podsumowanie per upgrade
    printf("  %2s | %-25s | %6s | %3s | %10s | %10s | %8s\n",
        "#", "Upgrade", "Bonus", "Max", "Contrib", "Cumul", "% cap");
    uint32_t cumul = 1;
    for (int i = 0; i < MaxUpgrades; i++) {
        Upgrade tmp = {upgrades[i].startingPrice, upgrades[i].startingBonus, upgrades[i].max, upgrades[i].max};
        uint32_t c = getOfficeUpgradeBonus(tmp);
        cumul += c;
        double pct = (double)cumul / maxCap * 100;
        printf("  %2d | %-25s | %6d | %3d | %10s | %10s | ",
            i, (const char*)upgradesNames[i], upgrades[i].startingBonus, upgrades[i].max,
            fmt(c), fmt(cumul));
        printBar(pct);
        printf(" %5.1f%%%s\n", pct, cumul > maxCap ? " OVER" : "");
    }
    printf("  TOTAL: %s / %s = %.1f%%\n", fmt(cumul), fmt(maxCap), (double)cumul/maxCap*100);

    // Krok po kroku
    printf("\n  Inbound po kazdym zakupie:\n");
    cumul = 1;
    uint8_t current[MaxUpgrades] = {};
    int pnum = 0;
    for (int i = 0; i < MaxUpgrades; i++) {
        for (int lvl = 1; lvl <= upgrades[i].max; lvl++) {
            pnum++;
            current[i] = lvl;
            uint32_t totalInbound = 1;
            for (int j = 0; j < MaxUpgrades; j++) {
                Upgrade tmp = {upgrades[j].startingPrice, upgrades[j].startingBonus, current[j], upgrades[j].max};
                totalInbound += getOfficeUpgradeBonus(tmp);
            }
            double pct = (double)totalInbound / maxCap * 100;
            printf("    #%3d %-25s lvl %2d => inbound=%6s ",
                pnum, (const char*)upgradesNames[i], lvl, fmt(totalInbound));
            printBar(pct);
            printf(" %5.1f%%%s\n", pct, totalInbound > maxCap ? " OVER" : "");
        }
    }

    // Overflow point
    memset(current, 0, sizeof(current));
    pnum = 0;
    bool foundOverflow = false;
    for (int i = 0; i < MaxUpgrades && !foundOverflow; i++) {
        for (int lvl = 1; lvl <= upgrades[i].max; lvl++) {
            pnum++;
            current[i] = lvl;
            uint32_t ti = 1;
            for (int j = 0; j < MaxUpgrades; j++) {
                Upgrade tmp = {upgrades[j].startingPrice, upgrades[j].startingBonus, current[j], upgrades[j].max};
                ti += getOfficeUpgradeBonus(tmp);
            }
            if (ti > maxCap) {
                printf("\n  OVERFLOW przy: %s lvl %d (zakup #%d)\n",
                    (const char*)upgradesNames[i], lvl, pnum);
                printf("  inbound=%s > capacity=%s\n", fmt(ti), fmt(maxCap));
                foundOverflow = true;
                break;
            }
        }
    }
    if (!foundOverflow) {
        printf("\n  Brak overflow - max inbound miesci sie w capacity\n");
    }

    return 0;
}
