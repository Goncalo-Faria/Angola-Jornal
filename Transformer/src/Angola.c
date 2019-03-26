#include <Angola.h>
#include <glib.h>
#include <stdio.h>

GHashTable * taghtable;

typedef struct angola {
    GString * id;
    GString * author;
    GList * tags;
    GString * title;
    GString * category;
    GList * lines;
} *Angola;

typedef struct tuple {
    GString * id;
    GString * title;
} *Tuple;

Tuple mkTuple(GString * id, GString * title){
    Tuple t = (Tuple)malloc(sizeof(struct tuple));
    t->id = g_string_new(id->str);
    t->title = g_string_new(title->str);
    return t;
}

void unmkTuple(Tuple t){
   g_string_free(t->id, TRUE);
   g_string_free(t->title, TRUE);
   g_free(t);
}

void stringTotalFree(GString * str){
    g_string_free(str, TRUE);
}

void addlink(GString * tag, GString* id, GString * title ){
    GString *a, *b;
    GHashTable *l,*tmp;
    Tuple t;

    if(g_hash_table_contains(taghtable,tag)){
        l = (GHashTable *)g_hash_table_lookup(taghtable,tag);
        t = mkTuple(id,title);
        g_hash_table_insert(l,t->id,t);

        g_hash_table_steal_extended(taghtable,tag, &a, &tmp);
        g_hash_table_insert(taghtable,a,l);
    } else {
        l = g_hash_table_new_full(g_string_hash,g_string_equal,NULL,unmkTuple);
        a = g_string_new(tag->str);
        t = mkTuple(id,title);
        g_hash_table_insert(l,t->id,t);
        g_hash_table_insert(taghtable,a,l);
    }

}

Angola mkAngola(){
    Angola a = (Angola)g_malloc( sizeof( struct angola));

    a->id = g_string_new("");
    a->author = g_string_new("");
    a->title = g_string_new("");
    a->category = g_string_new("");
    a->tags = NULL;
    a->lines = NULL;
    return a;
}

void unmkAngola(Angola a){
    g_string_free(a->id,TRUE);
    g_string_free(a->author,TRUE);
    g_string_free(a->title,TRUE);
    g_string_free(a->category,TRUE);
    g_list_free(a->tags);
    g_list_free(a->lines);
    g_free(a);
}

void printHTML(Angola a){
    GList *l;
    char *tmp;
    char *id = a->id->str;
    char *author = a->author->str;
    char *title = a->title->str;
    char *category = a->category->str;

    printf("<pub id=\"%s\">\n", id);

    printf("<!DOCTYPE html>\n");
    printf("<html>\n");
    printf("<head>\n");

    if (strstr(title, "---------"))
    {
        printf("<title></title>\n");
    }
    else
    {
        printf("<title>%s</title>\n", title);
    }

    printf("</head>\n");
    printf("<body>\n");

    printf("<h1><p align='center'><font color='#2874A6'>%s</font></p></h1>\n", title);

    printf("<div style='padding: 20px'>\n");
    printf("<author_date><h3><font color='#85C1E9'>%s</font></h3></author_date>\n", author);

    printf("<p><tags><b>Tags:</b> \n");
    for (l = a->tags; l != NULL; l = l->next)
    {
        addlink(l->data, a->id, a->title);
        tmp = g_string_free(l->data, FALSE);
        printf("<tag>#%s</tag> \n", tmp);
        g_free(tmp);
    }
    printf("</tags></p>\n");

    printf("<p><category><b>Categoria:</b> %s</category></p>\n", category);

    printf("</div>\n");

    printf("<text>\n");

    printf("<div style='float: left; width: 50");
    printf("%");
    printf(";'>\n");
    printf("<div style='padding: 20px'>\n");
    guint length = g_list_length(a->lines);
    int i = 0;

    for (l = a->lines; l != NULL; l = l->next)
    {
        tmp = g_string_free(l->data, FALSE);
        printf("<p align='justify'>%s</p>\n", tmp);

        if (i == length / 2)
        {
            printf("</div>");
            printf("</div>\n");

            printf("<div style='float: left; width: 49");
            printf("%");
            printf(";'>\n");
            printf("<div style='padding: 20px'>\n");
        }

        g_free(tmp);
        i++;
    }

    printf("</div>");
    printf("</div>\n");

    printf("</text>\n");

    printf("</pub>\n");

    printf("</body>\n");
    printf("</html>");
}

void addId(Angola a, char* str){
    g_string_append(a->id,str);
}

void addAuthor(Angola a, char* str){
    g_string_append(a->author,str);
}

void addTag(Angola a, char* str){
    a->tags = g_list_append(a->tags, g_string_new(str));
}

void addTitle(Angola a, char* str){
    g_string_append(a->title,str);
}

void addCategory(Angola a, char* str){
    g_string_append(a->category,str);
}

void addLine(Angola a, char* str){
    a->lines = g_list_append(a->lines, g_string_new(str));
}

void tag(){
    taghtable = g_hash_table_new_full(g_string_hash, g_string_equal, stringTotalFree, g_hash_table_destroy );
}

void title_index_function(gpointer key, gpointer value, gpointer user_data){
    GString* s = (GString*)key;
    GList* l = g_hash_table_get_values((GHashTable*)value);
    GList* cur;
    Tuple tmp;
    char buffer[2000];
    char *ptr;

    strcpy(buffer,s->str);
    for(ptr = buffer;*ptr != '\0'; ptr++ )
        if( *ptr == ' ' )
            *ptr = '-';

    printf("<pub id=\"%s\">\n",buffer);
    printf("\t<h1>%d</h1>\n",(int)g_list_length(l));
    for(cur = l; cur; cur = cur->next){
        tmp = (Tuple)cur->data;

        printf("\t\t<a href=\"%s.html\">%s</a>\n",tmp->id->str,tmp->title->str);
    }
    printf("</pub>\n");
}

void tag_index_function(gpointer key, gpointer value, gpointer user_data){
    GString* s = (GString*)key;
    char buffer[2000];
    char *ptr;

    strcpy(buffer,s->str);
    for(ptr = buffer;*ptr != '\0'; ptr++ )
        if( *ptr == ' ' )
            *ptr = '-';

    printf("\t\t<a href=\"%s.html\">%s</a>\n",buffer,s->str);
}


void trace(){

    printf("<pub id=\"tags\">\n");
    g_hash_table_foreach(taghtable,tag_index_function,NULL);
    printf("</pub>\n");

    g_hash_table_foreach(taghtable,title_index_function,NULL);

    g_hash_table_destroy(taghtable);

}
