#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/treasure.h"
#include "../include/input.h"
#include "../include/storage.h"

typedef struct { int row, col, found; } Hidden;
// So o teste le o arquivo persistido; a API de producao nao expoe tesouros ocultos.
static int hidden(const char *path, Hidden out[10]) {
    FILE *f = fopen(path, "r");
    char line[512];
    int v[10];
    assert(f);
    for (int i = 0; i < 4; i++) assert(fgets(line, sizeof line, f));
    assert(sscanf(line, "%d%d%d%d%d%d%d%d%d%d", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8], &v[9]) == 10);
    for (int i = 0; i < v[5]; i++) assert(fscanf(f, "%d%d%d", &out[i].row, &out[i].col, &out[i].found) == 3);
    assert(fclose(f) == 0);
    return v[5];
}

static int contains(Hidden *t, int n, int r, int c) {
    for (int i = 0; i < n; i++) if (t[i].row == r && t[i].col == c) return 1;
    return 0;
}

static const char *answer(const TreasureGame *g) {
    const char *q = getEnigmaQuestion(g);
    if (strstr(q, "5 + 3")) return "8";
    if (strstr(q, "3 * 4")) return "12";
    if (strstr(q, "15 / 3")) return "5";
    if (strstr(q, "12 - 3")) return "9";
    if (strstr(q, "4 + 3")) return "7";
    if (strstr(q, "2 * 3")) return "6";
    return "10";
}

static void exercise(difficulty level) {
    Player player = {"Explorador de teste", 0, level};
    TreasureGame *g, *copy;
    GameStatus a, b;
    Hidden t[10];
    int n, er = -1, ec = -1, count;
    char compass[64], filename[64];
    snprintf(filename, sizeof filename, "fixture-%d.sav", level);
    srand(400u + (unsigned)level);
    g = startTreasureGame(getGameConf(level), &player);
    assert(g && saveTreasureGame(g, filename));
    n = hidden(filename, t);
    assert(getGameStatus(g, &a));
    for (int i = 0; i < n; i++) {
        assert(getKnownCell(g, t[i].row, t[i].col) == '.');
        for (int j = 0; j < i; j++) assert(t[i].row != t[j].row || t[i].col != t[j].col);
    }
    assert(investigateCoordinate(g, -1, 0) == INVESTIGATE_INVALID);
    assert(investigateCoordinate(g, a.rows, 0) == INVESTIGATE_INVALID);
    assert(getGameStatus(g, &b) && a.attemptsLeft == b.attemptsLeft);
    for (int r = 0; r < a.rows && er < 0; r++) for (int c = 0; c < a.columns; c++)
        if (!contains(t, n, r, c)) {
            er = r;
            ec = c;
            break;
        }
    assert(investigateCoordinate(g, er, ec) == INVESTIGATE_EMPTY);
    assert(investigateCoordinate(g, er, ec) == INVESTIGATE_REPEATED);
    assert(getGameStatus(g, &b) && b.attemptsLeft == a.attemptsLeft - 1);
    assert(useRadar(g, er, ec, &count));
    { int expected = 0;
      for (int i = 0; i < n; i++) if (abs(t[i].row-er) <= 1 && abs(t[i].col-ec) <= 1) expected++;
      assert(count == expected); }
    assert(useRadar(g, er, ec, &count));
    assert(getGameStatus(g, &b) && b.radarUsed == 1);
    assert(!useRadar(g, t[0].row, t[0].col, &count));
    // Referencia deterministica: a primeira posicao na ordenacao linha/coluna.
    { int best=0, d=1000;
      for(int i=0;i<n;i++) { int v=abs(t[i].row-er)+abs(t[i].col-ec);
        if(v<d || (v==d && (t[i].row<t[best].row || (t[i].row==t[best].row && t[i].col<t[best].col)))) { best=i; d=v; } }
      assert(useCompass(g, er, ec, compass, sizeof compass));
      assert((t[best].row<er) == (strstr(compass,"Norte") != NULL));
      assert((t[best].row>er) == (strstr(compass,"Sul") != NULL));
      assert((t[best].col<ec) == (strstr(compass,"Oeste") != NULL));
      assert((t[best].col>ec) == (strstr(compass,"Leste") != NULL)); }
    assert(saveTreasureGame(g, "roundtrip.sav"));
    copy = loadTreasureGame("roundtrip.sav");
    assert(copy && getGameStatus(g,&a) && getGameStatus(copy,&b));
    assert(!strcmp(a.player.name,b.player.name) && a.player.level==b.player.level && a.attemptsLeft==b.attemptsLeft &&
           a.foundCount==b.foundCount && a.player.score==b.player.score && a.radarUsed==b.radarUsed && a.compassesLeft==b.compassesLeft);
    for (int r=0;r<a.rows;r++) for(int c=0;c<a.columns;c++) assert(getKnownCell(g,r,c)==getKnownCell(copy,r,c));
    assert(useRadar(copy,er,ec,&count) && getGameStatus(copy,&b) && b.radarUsed==1);
    while (useCompass(copy,er,ec,compass,sizeof compass)) { }
    assert(getGameStatus(copy,&b) && b.compassesLeft==0);
    freeTreasureGame(copy);
    for (int i=0;i<n;i++) {
        InvestigateResult result=investigateCoordinate(g,t[i].row,t[i].col);
        if (level==HARD) {
            assert(result==INVESTIGATE_CHALLENGE);
            assert(!saveTreasureGame(g,"pending.sav"));
            assert(investigateCoordinate(g,er,ec)==INVESTIGATE_ENDED);
            assert(resolveEnigma(g,answer(g))==1);
        } else assert(result==INVESTIGATE_TREASURE);
    }
    assert(getTreasureState(g)==TREASURE_WON && getGameStatus(g,&a));
    assert(a.player.score==n*100+a.attemptsLeft*10);
    assert(investigateCoordinate(g,er,ec)==INVESTIGATE_ENDED);
    assert(!useRadar(g,er,ec,&count) && !useCompass(g,er,ec,compass,sizeof compass));
    assert(registerRanking(g) && registerRanking(g));
    snprintf(filename,sizeof filename,"won-%d.sav",level);
    assert(saveTreasureGame(g,filename));
    freeTreasureGame(g);
    // Derrota sem depender da sorte: explorar exclusivamente casas vazias.
    g=startTreasureGame(getGameConf(level),&player);
    assert(g && saveTreasureGame(g,"loss-source.sav"));
    n=hidden("loss-source.sav",t);
    assert(getGameStatus(g,&a));
    for(int r=0;r<a.rows && getTreasureState(g)==TREASURE_RUNNING;r++)
        for(int c=0;c<a.columns && getTreasureState(g)==TREASURE_RUNNING;c++)
            if(!contains(t,n,r,c)) assert(investigateCoordinate(g,r,c)==INVESTIGATE_EMPTY);
    assert(getTreasureState(g)==TREASURE_LOST && getGameStatus(g,&b) && b.attemptsLeft==0);
    assert(investigateCoordinate(g,t[0].row,t[0].col)==INVESTIGATE_ENDED);
    assert(registerRanking(g));
    freeTreasureGame(g);
    printf("PASSOU: nucleo, dicas, vitoria, derrota e persistencia %s.\n",difficultyName(level));
}
int main(void) {
    int row,col,value;
    FILE *f;
    char line[16], temp[128];
    assert(parseCoordinate("b3",&row,&col) && row==2 && col==1);
    assert(!parseCoordinate("B3xyz",&row,&col));
    assert(!parseCoordinate("B999999999999999999999999",&row,&col));
    assert(!parseCoordinate("B0",&row,&col));
    assert(!parseCoordinate("B-1",&row,&col));
    assert(!parseInteger("1abc",0,4,&value));
    assert(!parseInteger("9999999999999999999999999",0,4,&value));
    assert(validPlayerName("Nome com espacos") && !validPlayerName("   "));
    f=tmpfile();
    assert(f);
    fputs("12345678901234567890\nOK\n",f);
    rewind(f);
    assert(readStreamLine(f,line,sizeof line)==-1);
    assert(readStreamLine(f,line,sizeof line)==1 && !strcmp(line,"OK"));
    fclose(f);
    for(int level=EASY;level<=HARD;level++) exercise((difficulty)level);
    // Falha de escrita efetiva: stream somente de leitura, ferror detectado.
    f=fopen("preserved.txt","w");
    assert(f);
    fputs("original",f);
    fclose(f);
    f=fopen("preserved.txt","r");
    assert(f);
    assert(fputs("nao pode",f)==EOF);
    assert(!finishStoredWrite(f,"preserved.txt","unused.tmp"));
    f=fopen("preserved.txt","r");
    assert(f && fgets(line,sizeof line,f) && !strcmp(line,"original"));
    fclose(f);
    assert(!beginStoredWrite("missing/sub/file",temp,sizeof temp));
    // Hard: erro realoca para casa oculta e livre.
    { Player p={"Enigma",0,HARD}; Hidden before[10],after[10];
      TreasureGame *g=startTreasureGame(getGameConf(HARD),&p);
      assert(g && saveTreasureGame(g,"hard-before.sav"));
      hidden("hard-before.sav",before);
      assert(investigateCoordinate(g,before[0].row,before[0].col)==INVESTIGATE_CHALLENGE);
      assert(resolveEnigma(g,"errada")==0 && getKnownCell(g,before[0].row,before[0].col)=='o');
      assert(saveTreasureGame(g,"hard-after.sav"));
      hidden("hard-after.sav",after);
      assert(after[0].row!=before[0].row || after[0].col!=before[0].col);
      assert(!contains(before,7,after[0].row,after[0].col));
      assert(getKnownCell(g,after[0].row,after[0].col)=='.');
      freeTreasureGame(g); }
    printf("PASSOU: entradas, EOF, escrita com falha e realocacao Hard.\n");
    return 0;
}
