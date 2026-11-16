// Injecao de falhas somente nas alocacoes dos nossos fontes, sem interceptar o CRT.
#undef malloc
#undef calloc
#undef realloc
#undef free
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/treasure.h"

static void *live[4096];
static int liveCount, failAfter = -1;
static int fail(void) {
    if (failAfter < 0) return 0;
    if (failAfter == 0) {
        failAfter = -1;
        return 1;
    }
    failAfter--;
    return 0;
}

static void track(void *p) { if (p) { assert(liveCount < 4096); live[liveCount++] = p; } }
static int locate(void *p) {
    for (int i=0;i<liveCount;i++) if(live[i]==p) return i;
    return -1;
}

void *atlasTestMalloc(size_t n) {
    void *p;
    if(fail()) return NULL;
    p=malloc(n);
    track(p);
    return p;
}

void *atlasTestCalloc(size_t n,size_t size) {
    void *p;
    if(fail()) return NULL;
    p=calloc(n,size);
    track(p);
    return p;
}

void *atlasTestRealloc(void *p,size_t n) {
    int index=locate(p);
    void *result;
    if(fail()) return NULL;
    result=realloc(p,n);
    if(result) {
        if(index>=0) live[index]=result;
        else track(result);
    }
    return result;
}

void atlasTestFree(void *p) {
    int index;
    if(!p) return;
    index=locate(p);
    assert(index>=0);
    live[index]=live[--liveCount];
    free(p);
}

static void solve(TreasureGame *g) {
    FILE *f;
    char line[256];
    int row[7],col[7],found;
    const char *solutions[]={"8","12","5","9","7","6","10"};
    assert(saveTreasureGame(g,"alloc-source.sav"));
    f=fopen("alloc-source.sav","r");
    assert(f);
    for(int i=0;i<4;i++) assert(fgets(line,sizeof line,f));
    for(int i=0;i<7;i++) assert(fscanf(f,"%d%d%d",&row[i],&col[i],&found)==3);
    fclose(f);
    for(int i=0;i<7;i++) {
        assert(investigateCoordinate(g,row[i],col[i])==INVESTIGATE_CHALLENGE);
        assert(resolveEnigma(g,solutions[i])==1);
    }
}

int main(void) {
    Player p={"Falhas de memoria",0,HARD};
    TreasureGame *g,*copy;
    int baseline;
    for(int cut=0;cut<16;cut++) {
        failAfter=cut;
        g=startTreasureGame(getGameConf(HARD),&p);
        failAfter=-1;
        freeTreasureGame(g);
        assert(liveCount==0);
    }
    g=startTreasureGame(getGameConf(HARD),&p);
    assert(g);
    assert(saveTreasureGame(g,"alloc-source.sav"));
    baseline=liveCount;
    for(int cut=0;cut<20;cut++) {
        failAfter=cut;
        copy=loadTreasureGame("alloc-source.sav");
        failAfter=-1;
        freeTreasureGame(copy);
        assert(liveCount==baseline);
    }
    for(int cut=0;cut<8;cut++) {
        failAfter=cut;
        (void)saveTreasureGame(g,"alloc-save.sav");
        failAfter=-1;
        assert(liveCount==baseline);
        copy=loadTreasureGame("alloc-source.sav");
        assert(copy);
        freeTreasureGame(copy);
    }
    solve(g);
    for(int cut=0;cut<12;cut++) {
        failAfter=cut;
        (void)registerRanking(g);
        failAfter=-1;
        assert(liveCount==baseline);
    }
    assert(registerRanking(g));
    freeTreasureGame(g);
    assert(liveCount==0);
    printf("PASSOU: falhas de alocacao na criacao, leitura, escrita e ranking; zero alocacoes pendentes.\n");
    return 0;
}