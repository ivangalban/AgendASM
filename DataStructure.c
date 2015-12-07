#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#define LENGHT 37

struct Trie
{
    struct Trie *next[LENGHT];
    int eok, childs,count;
    int *person;
};
int top;
struct Trie *names,*phones;
FILE *fp, *dir;
int nextPos(char s)
{
    int pos=10;
    if(s=='_') return 0;
    else if( s >= 'a' && s <= 'z') pos+=(s-'a'+1);
    else if(s >= 'A' && s <= 'Z') pos+=(s-'A'+1);
    else if(s >= '0' && s <= '9') pos=s-'0'+1;
    else return -1;
    return pos;
}

struct _person
{
    int* p;
    struct _person* next;
};
typedef struct _person Person;
struct _group
{
    int count;
    char* name;
    Person* first;
    struct _group* next;
};
typedef struct _group Group;
Group  *first,*last;
int countGroups;


void cleanTrie(struct Trie* node) { //ok
    int i;
    if (node) {
        for (i = 0; i < LENGHT; ++i) {
            if (node->next[i]) {
                cleanTrie(node->next[i]);
                free(&node->person[0]);
                free(&node->person[1]);
                free(node->next[i]);
            }
        }
    }
    node = 0;
}

void Erase()
{

}

void HELP()
{
    int i;
    char *txtHelp[100];

    txtHelp[0] = "ADD NOMBRE_DE_CONTACTO: TELEFONO;        Agregar un contacto.";

    txtHelp[1] = "REMOVE NOMBRE_DE_CONTACTO;               Eliminar un contacto.";

    txtHelp[2] = "UPDATE NOMBRE_DE_CONTACTO: TELEFONO;     Actualizar un contacto.";

    txtHelp[3] = "SHOW NOMBRE_DE_CONTACTO;                 Mostrar un contacto.";

    txtHelp[4] = "LIST;                                    Listar todos los contactos.";

    txtHelp[5] = "LOAD PATH;                               Guardar los contactos de la ruta PATH.";

    txtHelp[6] = "LOGOUT;                                  Cerrar sesion.";

    txtHelp[7] = "ADDGROUP NOMBRE_DE_GRUPO;                Crear un nuevo grupo.";

    txtHelp[8] = "REMOVEGROUP NOMBRE_DE_GRUPO;             Eliminar un grupo.";

    txtHelp[9] = "ADDTOGROUP NOMBRE: GRUPO;                Agregar contacto a un grupo.";

    txtHelp[10] = "LISTGROUP NOMBRE_DE_GRUPO;               Mostrar contactos asociados al grupo.";

    txtHelp[11] = "SEARCHNAME CADENA;                       Mostrar contactos que tienen en su                                              nombre a cadena como prefijo.";

    txtHelp[12] = "SEARCHPHONE CADENA;                      Mostrar contactos que tienen como                                               telefono a cadena como prefijo.";
 
    txtHelp[13] = "ABOUT;                                   Ver datos de los creadores.";

    printf("\n");
    for(i = 0; i < 14; i++)
        printf("%s\n\n", txtHelp[i]);
}

struct Trie* initTrie()   //ok
{
    int i;
    struct Trie* t=(struct Trie*)malloc(sizeof(struct Trie));
    t->eok = t->childs = t->count = 0;
    t->person = 0;
    for ( i = 0; i < LENGHT ; ++i) {
        t->next[i]=0;
    }
    return t;
}

void cleanGroups()
{
    if(!first) return;
    Group *cur=first;
    Group *tmp;
    Person *curp;
    Person *tmpp;
    while(cur)
    {
        tmp=cur;
        curp=cur->first;
         while (curp)
        {
            tmpp=curp;
            curp=curp->next;
            free(tmpp);
        }
        cur=cur->next;
        free(tmp);

    }

    first = 0;
    last = 0;
    countGroups = 0;
}

void initStructureGroup()
{
    cleanGroups();
    first=0;
    last=0;
    countGroups=0;
}

void initStructure() { //ok
    
    initStructureGroup();

    names = initTrie();
    phones = initTrie();
   
} 


//public
struct Trie* get(struct Trie *node,char *s) //ok
{
    int pos;
    struct Trie *cur = node;
    while (*s) {
        pos=nextPos(*s);
        if(pos == -1)   return 0;
        if (cur->next[pos])
            cur = cur->next[pos];
        else
            return 0;
        ++s;
    }
    return cur->eok?cur:0;
}
int* getPerson(char *s)//showPerson //ok
{
    struct  Trie *node = get(names,s);
    if(!node) return 0;
    int *p=(int*)malloc(3*sizeof(int));
    p[0]=1;
    p[1]=node->person[0];
    p[2]=node->person[1];
    return p;
}

void addtoTrie(struct Trie* node,int i,int *person) //inertPerson //ok
{

    char *s=(char*)person[i];
    struct Trie *cur = node;
    int pos;
    while (*s) {
        pos=nextPos(*s);
        cur->count++;
        if (cur->next[pos])
            cur = cur->next[pos];
        else {
            cur->childs++;
            cur->next[pos]=initTrie();
            cur = cur->next[pos];
        }
        ++s;
    }
    cur->eok = 1;
    cur->count++;
    cur->person = person;
}

int Check_Phone(char *s)
{
    int i;
    for(i = 0; i < strlen(s); i++)
        if(!isdigit(s[i]))
            return 0;
    return 1;
}

int Check_Name(char *s)
{
    int i;
    for(i = 0; i < strlen(s); i++)
        if(!isalpha(s[i]) && !s[i]=='_')
            return 0;
    return 1;
}

int addPerson(char *_name,char *_phone) //ok
{
    if(Check_Name(_name) == 0)   return 0;
    if(Check_Phone(_phone) == 0)   return 2;


    char* name = (char*)malloc((strlen(_name) + 1)*sizeof(char));
    char* phone = (char*)malloc((strlen(_phone) + 1)*sizeof(char));

    strcpy(name, _name);
    strcpy(phone, _phone);

    int length=strlen(phone);
    int i;
    for(i=0;i<length;++i){if(!isdigit(phone[i])) return 4;}
    int* person=(int*)malloc(2*sizeof(int));
    person[0]=(int)name;
    person[1]=(int)phone;
    if(!get(names,name))
    {
        if(!get(phones,phone))
        {
            addtoTrie(names,0,person);
            addtoTrie(phones,1,person);
            return 1;
        }
        return 2;
    }
    return 0;
}

int poss;
void show(struct Trie* node,int*contactos)
{
    if(node->eok==1)
    {
        contactos[poss++]=node->person[0];
        contactos[poss++]=node->person[1];
    }
    int i;
    for (i=0;i<LENGHT;++i)
    {
        if(node->next[i])
        {
            show(node->next[i],contactos);
        }
    }
}
int* LIST()//ok
{
    poss=1;
    int* contactos=(int*)malloc((2*names->count+1)*sizeof(int));
    contactos[0]=names->count;
    show(names,contactos);
    return contactos;
}
void removeAux(struct Trie *t,char *s) {//ok
    int i,pos;
    struct Trie* stack[strlen(s)+3];
    top = 0;
    stack[top++] = t;
    struct    Trie *cur = t;
    while (*s) {
        pos=nextPos(*s);
        cur = stack[top++] = cur->next[pos];
        ++s;
    }
    for (i = 0; i < top; ++i) {
        stack[i]->count--;
    }
    cur->eok=0;
    while (top > 1) {
        --s;
        pos=nextPos(*s);
        cur = stack[--top];
        if (cur->childs || cur->eok)
            return;

        stack[top - 1]->childs--;
        stack[top - 1]->next[pos] = 0;
        free(cur);
    }
}

void removePersonfromGroup(Group* group,char *name)
{
        Person* person=group->first;
        if(!person) return;
        if(!strcmp(name,(char*)person->p[1])) {group->first=person->next; group->count--; free(person); return; }
        while(person->next)
        {
            if(!strcmp(name,(char*)person->next->p[1]))
            {
                Person* tmp=person->next;
                person->next=person->next->next;
                group->count--;
                free(tmp);
                return;
            }
            person=person->next;
        }
       
}

void removePersonfromAllGroup(char* name)
{
    Group* cur=first;
    while(cur)
    {
       removePersonfromGroup(cur,name);
       cur=cur->next;
    }
}

int REMOVE_PERSON(char *s)//ok
{
    int* node = getPerson(s);
    if(node) {
        removeAux(names, (char *) node[1]);
        removeAux(phones, (char *) node[2]);
        free(&node[0]);
        free(&node[1]);
        free(node);
        removePersonfromAllGroup(s);
        return 1;}
    return 0;
}
int UPDATE(char* name,char* phone)  //ok
{
    if(Check_Name(name) == 0)    return 0;

    struct Trie *tmp=get(names,name);
    if(!tmp)return 0;
    if(Check_Phone(phone) == 0)   return 2;
    removeAux(phones,(char*)tmp->person[1]);
    free(&tmp->person[1]);
    char* _phone=(char*)malloc((strlen(phone)+1)*sizeof(char));
    strcpy(_phone,phone);
    tmp->person[1]=(int)_phone;
    addtoTrie(phones,1,tmp->person);
    return 1;
}
void printt(int *c)
{
    int i;
    for(i=0;i<c[0];i++)
    {
        printf("%s %s\n",(char*)c[2*i+1],(char*)c[2*i+2]);
    }
}

char* Tolower_String(char* s)
{
    int i;
    char* temp = (char*)malloc((strlen(s)+1) * sizeof(char));

    for(i = 0; i < strlen(temp); i++)
        temp[i] = tolower(s[i]);
    temp[i] = '\0';
    return temp;
}

void qSort(int* contactos, int lo, int hi ) { //ok

    int mid = lo,  l = lo, r = hi;
    char* al = NULL;char* am = NULL;char* ar = NULL;
    do {
        al=(char*)contactos[l];
        am=(char*)contactos[mid];
        ar=(char*)contactos[r];
        al = Tolower_String(al);
        am = Tolower_String(am);
        ar = Tolower_String(ar);
        while (strcmp(al,am)<0){ free(al);l+=2;al=(char*)contactos[l];al = Tolower_String(al);}
        while (strcmp(ar,am)>0){ free(ar);r-=2;ar=(char*)contactos[r];ar = Tolower_String(ar);}
        if ( l <= r ) {
            int tmp1=contactos[l];
            int tmp2=contactos[l+1];
            contactos[l]=contactos[r];
            contactos[l+1]=contactos[r+1];
            contactos[r]=tmp1;
            contactos[r+1]=tmp2;
            l+=2;
            r-=2;
        }

    }
    while ( l <= r );

    free(am);free(al);free(ar);

    if ( l < hi )qSort(contactos, l, hi );
    if ( lo < r ) qSort(contactos,lo, r );
}
void quickSort(int *contactos) { //ok
    qSort(contactos, 1, 2* contactos[0]-1);
}

int *searchByPreffix(struct Trie *node ,char* s)
{
    struct Trie *cur=node;
    int pos;
    int notFound=0;
    while (*s) {
        pos=nextPos(*s);
        if (cur->next[pos])
            cur = cur->next[pos];
        else {
            notFound=1;
            break;
        }
        ++s;
    }
    int *contactos;
    if(notFound)
    {
        contactos=(int*) malloc((1)*sizeof(int));
        contactos[0]=0;
        return contactos;
    }
    contactos=(int*) malloc((2*cur->count+1)*sizeof(int));
    poss=1;
    contactos[0]=cur->count;
    show(cur,contactos);
    return contactos;
}

int* SEARCH_BY_PHONE(char *number) //ok
{
    poss=1;
    if(!phones->count)
        return 0;
    int* locate=searchByPreffix(phones,number);
    if(locate[0]>1)
        quickSort(locate);
    return locate;
}
int* SEARCH_BY_NAME(char *name)//ok
{
    poss=1;
    if(!names->count)
        return 0;
    return searchByPreffix(names,name);
}



Group* getGroup(char* name)
{
    Group *cur=first;
    while(cur)
    {
        if(!strcmp(name,cur->name)) return cur;
        cur=cur->next;
    }
    return 0;
}

int AddGroup(char* name)
{
    if(getGroup(name)) return 4;
    ++countGroups;
    Group *newGroup=(Group*) malloc(sizeof(Group));
    newGroup->name=(char*)malloc((strlen(name)+1)*sizeof(char));
    strcpy(newGroup->name,name);
    newGroup->next=0;
    newGroup->first=0;
    newGroup->count=0;
    if(!first) {first=newGroup; last=newGroup; return 1;}
    last->next=newGroup;
    last=newGroup;
    return 1;
}
int removeGroup(char *name)
{
    Group *cur=first,*ant=0;
    Person *p;
    while(cur)
    {
        if(!strcmp(name,cur->name)){break;}
        ant=cur;
        cur=cur->next;
    }
    if(!cur) return 5;
    --countGroups;
    if(cur==first)
    {
        if(cur->next==0)
        {
            initStructureGroup();
        }
        else
        {
            first=first->next;
            free(ant);
        }
    }
    else if(cur->next==0)
    {
        ant->next=0;
        free(cur);
    }
    else
    {
        ant->next=cur->next;
        free(cur);
    }
    return 1;
}

int addPersontoGroup(char* _name,char* _group)
{
    if(!getPerson(_name))    return 0;
    Group *group=getGroup(_group);
    if(!group) return 5;
    Person* cur=group->first;
    Person* newPerson=(Person*)malloc(sizeof(Person));
    newPerson->next=0;
    newPerson->p=getPerson(_name);

    if(!group->first) {
        ++group->count;
        group->first=newPerson;
        return 1;    
    }
    if(!strcmp(_name,(char*)cur->p[1])) return 6;

    while(cur->next)
    {
        if(!strcmp(_name,(char*)cur->next->p[1])) return 6;
        cur=cur->next;
    }
    cur->next=newPerson;
    ++group->count;
    return 1;
}

int* ListGroup(char* group)
{
    Group* g=getGroup(group);
    if(!g) return 0;
    int *list=(int*)malloc((g->count*2+1)*sizeof(int));
    list[0]=g->count;
    Person *cur=g->first;
    int i=1;
    while(cur)
    {
        list[i++]=cur->p[1];
        list[i++]=cur->p[2];
        cur=cur->next;
    }
    return list;
}
int* groups(char* name)
{

    int *list = (int*) malloc((countGroups+1)*sizeof(int));
    list[0]=0;

    if(!getPerson(name))    return list;

    Group* cur=first;
    Person* person;
    int i=1;
    while(cur)
    {
        person=cur->first;
        while(person)
        {
            if(!strcmp(name,(char*)person->p[1]))
            {
                ++list[0];
                list[i++]=(int)cur->name;
                break;
            }
            person=person->next;
        }
        cur=cur->next;
    }
    return list;
}
int loadFile()
{
    if(!dir) return 3;
    initStructure();
    initStructureGroup();
    int count;

    char name[300];
    char phone[300];
    fscanf(dir,"%d",&count);
    while (count--)
    {
        fscanf(dir,"%s %s",name,phone);
        addPerson(name,phone);
    }
    char s[200];
    int n,m;
    fscanf(dir,"%d",&n);
    while(n--) {
        fscanf(dir, "%s %d", s, &m);
        AddGroup(s);
        while (m--) {
            fscanf(dir, "%s", name);
            addPersontoGroup(name, s);
        }
    }
    return 1;
}
void saveFile()
{
    int* contactos=LIST();
    int count=contactos[0];
    fprintf(dir,"%d ",count);
    int i;
    for(i=0;i<count;i++)
    {
        fprintf(dir,"%s ",(char*)contactos[2*i+1]);
        fprintf(dir,"%s ",(char*)contactos[2*i+2]);
    }
    fprintf(dir,"%d ",countGroups);
    Group* cur=first;
    while(cur)
    {
        fprintf(dir,"%s %d ",cur->name,cur->count);
        Person *p=cur->first;
        while (p){
            fprintf(dir,"%s ",(char*)p->p[1]);
            p=p->next;
        }
        cur=cur->next;
    }
}


void Put_Space(char* direction)
{
    int i;
    for(i = 0; i < strlen(direction); i++){
        if(direction[i] == '_')
            direction[i] = ' ';
    }
}

void SAVE(char* direction)
{
    Put_Space(direction);
    dir=fopen(direction,"w");
    saveFile();
    fclose(dir);
}
int LOAD(char* direction)
{
    Put_Space(direction);
    dir=fopen(direction,"r");
    int res=loadFile();
    fclose(dir);
    return res;
}

void LOGIN()
{
    initStructure();
    char name[500];
    printf("LOGIN:\n");
    scanf("%s",name);
    char password[200];
    const char ENTER = 13;
    int current=0;
    fp=fopen(name,"r");
    printf(fp?"ENTER PASSWORD:\n":"YOU ARE A NEW USER\nCREATE PASSWORD:\n");
    while((password[current++] = getch()) != ENTER)
    {
        if(password[current-1]==8)
        {
            if(current>0)--current;
            if(current>0)--current;
            printf("\b \b");
        }
        else{
            putch('*');
        }
    }
    putch('\n');
    password[current-1]='\0';
    if(fp)
    {
        char validPassword[200];
        fscanf(fp,"%s",validPassword);
        if(strcmp(password,validPassword))
        {
            printf("INVALIDPASSWORD\n");
            fclose(fp);
            LOGIN();
        }
        else
        {
            dir=fp;
            loadFile();
            fclose(fp);
            fclose(dir);
            fopen(name,"w");
            fprintf(fp,"%s ",password);
        }
    }
    else
    {
        fclose(fp);
        fp=fopen(name,"w");
        fprintf(fp,"%s ",password);
    }
}

void EXIT_AGENDASM()
{
    dir = fp;
    saveFile();
    fclose(dir);
    fclose(fp);
    cleanTrie(names);
    cleanTrie(phones);
    cleanGroups();
}

void LOGOUT()
{
    EXIT_AGENDASM();
}

