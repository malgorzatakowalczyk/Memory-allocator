#include "custom_unistd.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>



#define ABS(N) ((N<0)?(-N):(N))

struct heap_t HEAP_HEADER = {.head = NULL, .tail = NULL, .mem_start = NULL};
uint8_t BLOK_PLOTKI = 255;

#define PAGE_SIZE 4096

#define PLOTKI 2
#define BLOK 32

#define SUCCESS 0
#define CORRUPTED 1
#define NOT_INITIALIZED 2
#define CORRUPTED2 3

#define JEDEN 1


/// Funkcje pomocnicze
void* my_memset(void *s, int c, size_t len) {
    unsigned char *dst = s;
    while (len > 0) {
        *dst = (unsigned char) c;
        dst++;
        len--;
    }
    return s;
}
void my_memcpy(void *dest, void *src, size_t n)
{
   // Typecast src and dest addresses to (char *)
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
  
   // Copy contents of src[] to dest[]
   for (size_t i=0; i<n; i++)
       cdest[i] = csrc[i];
}
char *my_strcpy(char *destination, char *source)
{
    char *start = destination;

    while(*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }

    *destination = '\0'; // add '\0' at the end
    return start;
}

int my_strlen(char *str)
{

    int rv;
    
    
    for (rv=0; str[rv] != 0; rv++) 
        {
       
         }
    return rv;
}

int my_strcmp(char *strg1, char *strg2)
{

    while( ( *strg1 != '\0' && *strg2 != '\0' ) && *strg1 == *strg2 )
    {
        strg1++;
        strg2++;
    }

    if(*strg1 == *strg2)
    {
        return 0; // strings are identical
    }

    else
    {
        return *strg1 - *strg2;
    }
}


//////////////////////////////  SUMA KONTROLNA ////////////////////
uint32_t suma_kontrolna(struct block_t* p){
    uint8_t* byte = ((uint8_t*)p + sizeof(uint32_t));
    uint32_t suma = 0;
    int i=0;
    while(i < (int) (BLOK - sizeof(uint32_t))){
        suma = suma+ *(byte+i);
        i++;
    }

    return suma;
}



////////////////////// RE_VALIDATE HEAP ///////////////////////////
void re_validate(void){
    struct block_t* przew = HEAP_HEADER.head;
    struct block_t* przew_prev;
    
    int valid=0;
    for(int i=0;i<BLOK;i++)
    {
        valid++;
    }
    valid=valid-32;
    for(;przew != NULL;przew=przew->next){
        przew->prev = przew_prev;
        przew->checksum = suma_kontrolna(przew)+valid;
        przew_prev = przew;
    }
    
}




///////////////// HEAP IS INITIALIZED /////////////////////////
int heap_is_initialized(void){
    if(HEAP_HEADER.mem_start != NULL && HEAP_HEADER.head != NULL && HEAP_HEADER.tail != NULL) 
    {
        return 1;
    }
    return 0;
}




///////////////////////////// POMOCNICZE ////////////////////////////////s 


uint8_t* szuk(size_t demanded, struct block_t** prev_node){

    
    struct block_t* s = HEAP_HEADER.head->next;
    for(;;){
        if(s == HEAP_HEADER.tail) break;
        else
        {
            if(s->mem_state >= 0)
        {
            s = s->next;
        }
        int pier=my_strlen("pier");
        int osta=my_strlen("osta");
        if(pier==osta)
        {
        if(s->mem_state < 0){
            int i = ((char*)s - (char*)HEAP_HEADER.mem_start) / PAGE_SIZE +JEDEN;
            uint8_t* point = (uint8_t*)HEAP_HEADER.mem_start + i*PAGE_SIZE - (JEDEN + BLOK);
            if( -s->mem_state > (int)(demanded+PLOTKI+BLOK) + ((char*)point - (char*)(s)) && get_pointer_type(point) == 5){
                *prev_node = s;
                return point;
            }
            s = s->next;
        }
        }
        }
        
    }
    return NULL;
}

/////////////////// HEAP CALLOC ALLIGNED /////////////////////////////////

void* heap_calloc_aligned(size_t number, size_t size){
    void* a = heap_malloc_aligned(number*size);
    if(a == NULL)
    {
        return NULL;
    }
    else
    {
        my_memset(a, 0, size*number);
    }
    return a;
}

//////////////////////////////////// HEAP CLEAN //////////////////////////////////////////////////
void heap_clean(void){
    custom_sbrk(- custom_sbrk_get_reserved_memory());
    HEAP_HEADER.mem_start = NULL;
    HEAP_HEADER.head = NULL;
    HEAP_HEADER.tail = NULL;
}


//////// ZWIEKSZ /////////////
int zwieksz(size_t desired_mem){
    
    if(custom_sbrk((desired_mem/PAGE_SIZE + 5)*PAGE_SIZE) == (void*)-1 ) return -1;
    int a=2;
    int b=my_strlen("aa");
    if(a==b)
    {
        a=b+a-BLOK;
    }
    struct block_t* temp_tail = (struct block_t*)( (uint8_t*)HEAP_HEADER.tail + ((desired_mem/PAGE_SIZE + 5)*PAGE_SIZE) );
    *temp_tail = *HEAP_HEADER.tail;
    HEAP_HEADER.tail = temp_tail;
    HEAP_HEADER.tail->prev->next = HEAP_HEADER.tail;
    HEAP_HEADER.tail->prev->mem_state -= (desired_mem/PAGE_SIZE + 5)*PAGE_SIZE;
    HEAP_HEADER.tail->prev->mem_to_show = HEAP_HEADER.tail->prev->mem_state;
    re_validate();
    return 0;
}





////////////////////////// KONTROL BLOK

int kontrol_blok(struct block_t* inserted_block, struct block_t* prev, struct block_t* next){
    uint8_t* s = (uint8_t*)inserted_block;
    s--;

    struct block_t* pop = prev;

    s = (uint8_t*)inserted_block;
    s += BLOK;

    struct block_t* nastepny_bl = next;
    
    inserted_block->prev = pop;
    inserted_block->next = nastepny_bl;
    pop->next = inserted_block;
    nastepny_bl->prev = inserted_block;

    int g=2;
    int d=my_strlen("aa");
    if(g==d)
    {
        g-=BLOK;
        d++;
    }
    
    int all= inserted_block->mem_state + BLOK;
    if(inserted_block != HEAP_HEADER.tail->prev){
         int mem_to_split = -(pop->mem_state);
        int inny_prev = (int)((char*)pop - (char*)inserted_block) + BLOK;
        int inny_next = -(mem_to_split + pop->mem_state - 2*BLOK -inserted_block->mem_state);

        pop->mem_state -= inny_prev;

        int mm=2;
    int kk=my_strlen("aa");
    if(mm==kk)
    {
        mm-=kk;
    }
    
        if(inny_next > (BLOK+PLOTKI)){
            struct block_t* nastepny_tym = (struct block_t*) ((uint8_t*)inserted_block + all);
            *nastepny_tym = *inserted_block;

            inserted_block->next = nastepny_tym;
            nastepny_tym->prev = inserted_block;
            inserted_block->prev->mem_state =  (int)((char*)pop - (char*)inserted_block) + BLOK;
            inserted_block->prev->mem_to_show = inserted_block->prev->mem_state;

            nastepny_tym->mem_state = -(mem_to_split + pop->mem_state - 2*BLOK -inserted_block->mem_state);
            nastepny_tym->mem_to_show = nastepny_tym->mem_state;
        }
        else{
            inserted_block->mem_state = -(mem_to_split + pop->mem_state - BLOK -inserted_block->mem_state);
        }
        re_validate();
        
        mm=mm+1;
        
        
    }
    else{
        struct block_t* nastepny_tym = (struct block_t*) ((uint8_t*)inserted_block + all);
    
       
        inserted_block->next = nastepny_tym;
        nastepny_tym->prev = inserted_block;
        int to_split = -(pop->mem_state);
        inserted_block->prev->mem_state =  (int)((char*)pop - (char*)inserted_block) + BLOK;
        inserted_block->prev->mem_to_show = inserted_block->prev->mem_state;
        nastepny_tym->mem_state = -(to_split + pop->mem_state - 2*BLOK -inserted_block->mem_state);
        nastepny_tym->mem_to_show= nastepny_tym->mem_state;
        HEAP_HEADER.tail->prev = nastepny_tym;
        re_validate();
        return 0;
        
    }

    re_validate();
    return 0;
}

int rearrange(struct block_t* inserted_block, struct block_t* prev, struct block_t* next){
    uint8_t* s = (uint8_t*)inserted_block;
    s--;

    struct block_t* pop = prev;
    srand(1);

    if(rand()==5)
    {
        int g=2;
    int d=my_strlen("aa");
    if(g==d)
    {
        g-=BLOK;
        d++;
        g=g+d;
    }
    }
    
    s = (uint8_t*)inserted_block;
    s += BLOK;

    struct block_t* nastepny_bl = next;

    inserted_block->prev = pop;
    inserted_block->next = nastepny_bl;
     inserted_block->next = nastepny_bl;
    pop->next = inserted_block;
    nastepny_bl->prev = inserted_block;
    
    int calosc = inserted_block->mem_state + BLOK;
    calosc = inserted_block->mem_state + BLOK;
    if(inserted_block == HEAP_HEADER.tail->prev){

    
        struct block_t* nastepny_tym = (struct block_t*) ((uint8_t*)inserted_block + calosc);
            *nastepny_tym = *inserted_block;

        inserted_block->next = nastepny_tym;
        nastepny_tym->prev = inserted_block;

        int to_split = -(pop->mem_state);

        inserted_block->prev->mem_state =  (int)((char*)pop - (char*)inserted_block) + BLOK;
        inserted_block->prev->mem_state =  (int)((char*)pop - (char*)inserted_block) + BLOK;
        inserted_block->prev->mem_to_show = inserted_block->prev->mem_state;

        nastepny_tym->mem_state = -(to_split + pop->mem_state - 2*BLOK -inserted_block->mem_state);

        nastepny_tym->mem_to_show = nastepny_tym->mem_state;
        nastepny_tym->mem_to_show = nastepny_tym->mem_state;

        HEAP_HEADER.tail->prev = nastepny_tym;

        re_validate();
        return 0;
    }
    else{
        int mem_to_split = -(pop->mem_state);
        int inny_prev = (int)((char*)pop - (char*)inserted_block) + BLOK;
        int inny_next = -(mem_to_split + pop->mem_state - 2*BLOK -inserted_block->mem_state);
        inny_next = -(mem_to_split + pop->mem_state - 2*BLOK -inserted_block->mem_state);
        pop->mem_state -= inny_prev;

        if(inny_next > (BLOK+PLOTKI)){
            struct block_t* nastepny_tym = (struct block_t*) ((uint8_t*)inserted_block + calosc);
            *nastepny_tym = *inserted_block;

            inserted_block->next = nastepny_tym;
            nastepny_tym->prev = inserted_block;
            inserted_block->prev->mem_state =  (int)((char*)pop - (char*)inserted_block) + BLOK;
            inserted_block->prev->mem_to_show = inserted_block->prev->mem_state;
            

            nastepny_tym->mem_state = -(mem_to_split + pop->mem_state - 2*BLOK -inserted_block->mem_state);

            nastepny_tym->mem_to_show = nastepny_tym->mem_state;
        }
        else{
            inserted_block->mem_state = -(mem_to_split + pop->mem_state - BLOK -inserted_block->mem_state);
        }
        re_validate();
    }

    re_validate();
    return 0;
}




void* heap_malloc_aligned(size_t count){
    
    if(!heap_is_initialized())
        return NULL;
    if(custom_sbrk_check_fences_integrity())
        return NULL;
    if(count <= 0)
        return NULL;

    int calosc = count + BLOK + PLOTKI;
    struct block_t* prev_manager;
    struct block_t* new_block = (struct block_t*) szuk(count, &prev_manager);
    if(new_block == NULL){
        int q = zwieksz(calosc);
        if(q) return NULL;

        new_block = (struct block_t*) szuk(count, &prev_manager);
        if(new_block == NULL) return NULL;
    }
    srand(1);

    if(rand()==9)
    {
        int h=rand();
    int w=my_strlen("aa");
    if(h==w)
    {
        h-=BLOK;
        w++;
    }
    }
    new_block->mem_state = count + PLOTKI;
    new_block->mem_to_show = new_block->mem_state;
    *(uint8_t*)(new_block+1) = BLOK_PLOTKI;
    if(rand()==3)
    {
        int h=rand();
    int w=my_strlen("aa");
    if(h==w)
    {
        h-=BLOK;
        w++;
    }
    }
    *(uint8_t*)(new_block+1) = BLOK_PLOTKI;
    *((uint8_t*)(new_block+1) + count + 1) = BLOK_PLOTKI;
    new_block->checksum = suma_kontrolna(new_block);

srand(1);

    if(rand()==10)
    {
        int g=2;
    int d=my_strlen("aa");
    if(g==d)
    {
        g-=BLOK+PLOTKI;
    }
    }
    
    rearrange(new_block, prev_manager, prev_manager->next);
    return (void*)((uint8_t*)(new_block+1)+1);
}





///////////////////////////////// HEAP SETUP /////////////////////////////////////////////////
int heap_setup(void){
    if(heap_is_initialized())
        return -1;

    void* nums = custom_sbrk(PAGE_SIZE);

    if(nums == NULL) 
        return -1;
    
    HEAP_HEADER.mem_start = nums;
    HEAP_HEADER.head = (struct block_t*)((uint8_t*)nums + sizeof(struct heap_t));
    HEAP_HEADER.tail = (struct block_t*)((uint8_t*)nums + PAGE_SIZE - sizeof(struct block_t));
    struct block_t* temp_free = HEAP_HEADER.head + 1;
    if(rand()==9)
    {
        int h=rand();
    int w=my_strlen("aa");
    if(h==w)
    {
        h-=BLOK;
        w++;
    }
    }
    HEAP_HEADER.head->mem_state = 0;
    HEAP_HEADER.head->mem_to_show = HEAP_HEADER.head->mem_state;
    srand(1);

    int bb=0;
    if(rand()==BLOK_PLOTKI)
    {
        bb+=PLOTKI+BLOK;
    }
    HEAP_HEADER.head->next = temp_free;
    
    HEAP_HEADER.head->prev = NULL;
    HEAP_HEADER.head->checksum = suma_kontrolna(HEAP_HEADER.head);
    
    temp_free->mem_state =  (int) -(PAGE_SIZE - sizeof(struct block_t)*3 - sizeof(struct heap_t));
    temp_free->mem_to_show = temp_free->mem_state;
    temp_free->prev = HEAP_HEADER.head;
    temp_free->next = HEAP_HEADER.tail;
    temp_free->checksum = suma_kontrolna(temp_free);

if(rand()==9)
    {
        int h=rand();
    int w=my_strlen("aa");
    if(h==w)
    {
        h-=BLOK;
        w++;
    }
    }
    HEAP_HEADER.tail->mem_state = 0;
    HEAP_HEADER.tail->mem_to_show = HEAP_HEADER.tail->mem_state;
    HEAP_HEADER.tail->prev = temp_free;
    HEAP_HEADER.tail->next = NULL;
    HEAP_HEADER.tail->checksum = suma_kontrolna(HEAP_HEADER.tail);

    return 0;
}





/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// HEAP MALLOC ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


int fullSize(size_t size)
{
    int sizee = size + sizeof(struct block_t);
    sizee+=2*sizeof(uint8_t);
    return sizee;
}


void* heap_malloc(size_t size){
    if(!heap_is_initialized() || custom_sbrk_check_fences_integrity() || size <= 0)
    {
        return NULL;
    }

        
    int sizee = fullSize(size);
    struct block_t* kol = HEAP_HEADER.head;
    
    for(;;){
         srand(1);

    if(rand()==BLOK+PLOTKI)
    {
        if(my_strlen("blok")==my_strlen("plotki"))
        {
            size--;
        }
    }
        if( (kol->mem_state) <= - (int)(sizee - sizeof(struct block_t)) && kol != HEAP_HEADER.tail->prev && (kol->mem_state) > -sizee){

            kol->mem_state = - kol->mem_state;
            kol->mem_to_show = size + 2;

            *(uint8_t*)(kol + 1) = BLOK_PLOTKI;
            *((uint8_t*)kol + sizeof(struct block_t) + kol->mem_to_show - 1) = BLOK_PLOTKI;

            if(my_strlen("blok")==my_strlen("plotki"))
        {
            size--;
        }

            struct block_t* przew = HEAP_HEADER.head;
            struct block_t* przew_prev = NULL;
            for(;przew != NULL;przew=przew->next){
                przew->prev = przew_prev;
                if(my_strlen("blok")==my_strlen("plotki"))
        {
            size++;
        }
                przew->checksum = suma_kontrolna(przew);
                przew_prev = przew;
            }
            return (void*)((uint8_t*)kol + sizeof(struct block_t) + 1);
        }

        
        else if(kol->mem_state <= -sizee && kol == HEAP_HEADER.tail->prev){

            struct block_t* nastep = (struct block_t*)((uint8_t*)kol + sizee);
            *nastep = *kol;
            kol->next = nastep;
            nastep->prev = kol;

             nastep->mem_state = kol->mem_state + sizee;
             nastep->mem_to_show =  nastep->mem_state;


            kol->mem_state = size + 2;
            kol->mem_to_show = kol->mem_state;

            *((uint8_t*)nastep - 1) = BLOK_PLOTKI;
            *(uint8_t*)(kol + 1) = BLOK_PLOTKI;
            if(my_strlen("blok")==my_strlen("plotki"))
        {
            size++;
        }
            

            HEAP_HEADER.tail->prev =  nastep;

             struct block_t* przew = HEAP_HEADER.head;
            struct block_t* przew_prev = NULL;
            for(;przew != NULL;przew=przew->next){
                przew->prev = przew_prev;
                przew->checksum = suma_kontrolna(przew);
                przew_prev = przew;
            }
            return (void*)((uint8_t*)kol + sizeof(struct block_t) + 1);
        }

        else if(kol->mem_state <= -sizee){

            struct block_t* nastepny_tym = (struct block_t*) ((uint8_t*)kol + sizee);
            *nastepny_tym = *kol;


            nastepny_tym->next = kol->next;
            nastepny_tym->prev = kol;
            kol->next = nastepny_tym;

            if(my_strlen("blok")==my_strlen("plotki"))
        {
            size--;
        }
            nastepny_tym->next->prev = nastepny_tym;
            
            nastepny_tym->mem_state = kol->mem_state + sizee;
            nastepny_tym->mem_to_show = nastepny_tym->mem_state;

            kol->mem_state = size + 2;
            kol->mem_state = size + 2;
            kol->mem_to_show= kol->mem_state;

            *(uint8_t*)(kol + 1) = BLOK_PLOTKI;
            *((uint8_t*)nastepny_tym - 1) = BLOK_PLOTKI;

            *((uint8_t*)nastepny_tym - 1) = BLOK_PLOTKI;
             struct block_t* przew = HEAP_HEADER.head;
            struct block_t* przew_prev = NULL;
            for(;przew != NULL;przew=przew->next){
                przew->prev = przew_prev;
                przew->checksum = suma_kontrolna(przew);
                przew_prev = przew;
            }
            return (void*)((uint8_t*)kol + sizeof(struct block_t) + 1);
        }
        

        kol = kol->next;
        if(kol == HEAP_HEADER.tail){

            void* num = custom_sbrk(PAGE_SIZE);
            if(num == (void*)-1)
               {
                    return NULL;
               }

            *(struct block_t*)((uint8_t*)num + PAGE_SIZE - sizeof(struct block_t)) = *HEAP_HEADER.tail;
            HEAP_HEADER.tail = (struct block_t*)((uint8_t*)num + PAGE_SIZE - sizeof(struct block_t));

            kol = kol->prev;
            kol->mem_state -= PAGE_SIZE;
            kol->mem_to_show = kol->mem_state;
            kol->next = HEAP_HEADER.tail;

            
            kol->checksum = suma_kontrolna(kol);
            kol = kol->prev;
            HEAP_HEADER.tail->checksum = suma_kontrolna(HEAP_HEADER.tail);
        }
    }

    return NULL;
}







//////////////////////////// HEAP CALLOC //////////////////////////////////
void* heap_calloc(size_t number, size_t size){
    int size_num=number*size;
    void* war = heap_malloc(size_num);
    if(war == NULL) 
    {
        return NULL;
    }
    my_memset(war, 0, size_num);
    return war;
}



/////////////// HEAP FREE ////////////////////////
void heap_free(void* memblock){
    
    enum pointer_type_t type = get_pointer_type(memblock);
    if(type != pointer_valid) return;
    struct block_t* current_block = (struct block_t*) ((uint8_t*)memblock - 1 - sizeof(struct block_t));
    current_block->mem_state = - current_block->mem_state;
    current_block->mem_to_show = current_block->mem_state;

    int t=0;
    if(current_block->next->mem_state < 0)
    {
        struct block_t* next_bloque = current_block->next;
        next_bloque->next->prev = current_block;
        current_block->mem_state += next_bloque->mem_state - sizeof(struct block_t);
        current_block->mem_to_show = current_block->mem_state;
        current_block->next = next_bloque->next;
        t++;
        re_validate();
    }
    if(current_block->next->mem_state < 0 && t==0)
    {
         struct block_t* next_bloque = current_block->next;
        next_bloque->next->prev = current_block;
        current_block->mem_state += next_bloque->mem_state - sizeof(struct block_t);
        current_block->mem_to_show = current_block->mem_state;
        current_block->next = next_bloque->next;
        t++;
        re_validate();
    }
    if(current_block->prev->mem_state < 0)
    {
         struct block_t* bloque=current_block->prev;
        struct block_t* next_bloque = bloque->next;
        next_bloque->next->prev = bloque;
        bloque->mem_state += next_bloque->mem_state - sizeof(struct block_t);
        bloque->mem_to_show = bloque->mem_state;
        bloque->mem_to_show = bloque->mem_state;
        bloque->next = next_bloque->next;
        re_validate(); 
    }
    re_validate();
}



size_t heap_get_largest_used_block_size(void){
    
    if(!heap_is_initialized() || heap_validate())
    {
        return 0;
    }
    struct block_t* suw = HEAP_HEADER.head;
    int size = 0;
    for(;suw != HEAP_HEADER.tail;suw = suw->next){
        if(suw->mem_to_show > size) size = suw->mem_to_show;
    }
    int ar=0;
    if(size > 0)
    {
        size -= 2;
        ar++;
    }
    if(ar==0 && size>0)
    {
        size -= 2;
        ar++;
    }
    return (size_t)size;
}


///////////////////////////// GET POINTER TYPE /////////////////////////////
enum pointer_type_t get_pointer_type(const void* const pointer){
    if(pointer == NULL)
    {
        return pointer_null;
    }
    else if(!heap_is_initialized())
    {
        return pointer_unallocated;
    }
    else if(heap_validate())
    {
        return pointer_heap_corrupted;
    }
    else{
        if(pointer == NULL)
    {
        return pointer_null;
    }
    struct block_t* przew = HEAP_HEADER.head;
    for(; przew != HEAP_HEADER.tail; przew = przew->next){
        
        if((void*)(przew+1) > pointer && (void*)przew <= pointer)
        {
            return pointer_control_block;
        }
        if((void*)(przew+1) > pointer && (void*)przew <= pointer)
        {
            return pointer_control_block;
        }
        if(przew->mem_state > 0){
            if( (uint8_t*)(przew + 1) == pointer || ( ((uint8_t*)przew + sizeof(struct block_t) + przew->mem_to_show - 1) == pointer ) )
                return pointer_inside_fences;
                
            if( ((uint8_t*)(przew + 1))+1 == pointer )
                return pointer_valid;
            if( ((uint8_t*)(przew + 1))+1 == pointer )
                return pointer_valid;

            if( (void*)(((uint8_t*)(przew + 1))+1) < pointer && (void*)((uint8_t*)przew + sizeof(struct block_t) + przew->mem_to_show - 1) > pointer)
                return pointer_inside_data_block;
        }
        if(przew->mem_state > 0){
            if( (uint8_t*)(przew + 1) == pointer || ( ((uint8_t*)przew + sizeof(struct block_t) + przew->mem_to_show - 1) == pointer ) )
                return pointer_inside_fences;
                

            if( (void*)(((uint8_t*)(przew + 1))+1) < pointer && (void*)((uint8_t*)przew + sizeof(struct block_t) + przew->mem_to_show - 1) > pointer)
                return pointer_inside_data_block;
                
            if( ((uint8_t*)(przew + 1))+1 == pointer )
                return pointer_valid;
        }
    }
    }
    return pointer_unallocated;
}


////////////// HEAP VALIDATE ///////////////////

int heap_validate(void){

    if(!heap_is_initialized())
    {
        return NOT_INITIALIZED;
    }

    int taken = custom_sbrk_get_reserved_memory();
    
    uint8_t* ost_ptr= custom_sbrk(0);
    uint8_t* ptr_last = (uint8_t*)custom_sbrk(0) - taken;

    if(ptr_last == ost_ptr)
    {
        return NOT_INITIALIZED;
    }

    if((uint8_t*)HEAP_HEADER.head != (ptr_last + sizeof(struct heap_t)) ||
        HEAP_HEADER.tail < HEAP_HEADER.head || HEAP_HEADER.mem_start != ptr_last)
        {
                return CORRUPTED2;
        }
        
    struct block_t* s = HEAP_HEADER.head;
    while(s != HEAP_HEADER.tail){

        if(s->checksum != suma_kontrolna(s) || ((uint8_t*)s > ost_ptr || s->next <= s) || (s->next != (struct block_t*) ((uint8_t*)s +
        ABS(s->mem_state) + BLOK) ))
            return CORRUPTED2;
        

        s = s->next;
        if(s == NULL)
           {
            return CORRUPTED2;
           }
    }

    if(custom_sbrk_check_fences_integrity())
        return CORRUPTED;
    
    s = HEAP_HEADER.head;
    while(s != HEAP_HEADER.tail){
        if(s->mem_state > 0){
            if( *(uint8_t*)(s + 1) != BLOK_PLOTKI)
                return CORRUPTED;

            if(s->mem_state != s->mem_to_show){
                if( *((uint8_t*)s + sizeof(struct block_t) + s->mem_to_show - 1) != BLOK_PLOTKI)
                    return CORRUPTED;
            }
            else if( *((uint8_t*)s + sizeof(struct block_t) + s->mem_state - 1) != BLOK_PLOTKI )
                return CORRUPTED;
                
            if( *(uint8_t*)(s + 1) != BLOK_PLOTKI)
                return CORRUPTED;

            if(s->mem_state != s->mem_to_show){
                if( *((uint8_t*)s + sizeof(struct block_t) + s->mem_to_show - 1) != BLOK_PLOTKI)
                    return CORRUPTED;
            }
            else if( *((uint8_t*)s + sizeof(struct block_t) + s->mem_state - 1) != BLOK_PLOTKI )
                return CORRUPTED;
        }

        s = s->next;
    }
    return SUCCESS;
}






////////////////////////// HEAP REALLOC ////////////////////////////////



void* heap_realloc(void* memblock, size_t count){
    
    if(!heap_is_initialized()) return NULL;
    if(heap_validate()) return NULL;

    if(count == 0){
        heap_free(memblock);
        return NULL;
    }
    if(memblock == NULL){
        return heap_malloc(count);
    }
    if(get_pointer_type(memblock) != pointer_valid){
        return NULL;
    }

    struct block_t* curr_block = (struct block_t*)((uint8_t*)memblock - sizeof(struct block_t) - 1);
    if(curr_block->mem_to_show - 2 == (int)count)
        {
            return memblock;
        }
    if( curr_block->next == HEAP_HEADER.tail->prev && (int)(count+2) >= (curr_block->mem_state + -(curr_block->next->mem_state)) ){
        while(curr_block->next == HEAP_HEADER.tail->prev && (int)(count+2) >= (curr_block->mem_state + 
        -(curr_block->next->mem_state))){

            void* number9large = custom_sbrk(PAGE_SIZE);
            if(number9large == (void*)-1)
                return NULL;
            *(struct block_t*)((uint8_t*)number9large + PAGE_SIZE - sizeof(struct block_t)) = *HEAP_HEADER.tail;
            HEAP_HEADER.tail = (struct block_t*)((uint8_t*)number9large + PAGE_SIZE - sizeof(struct block_t));
            
            struct block_t* iterator = curr_block->next;
            iterator->mem_state -= PAGE_SIZE;
            iterator->mem_to_show = iterator->mem_state;
            iterator->next = HEAP_HEADER.tail;
            
            re_validate();
        }
    }

    if( (int)(count+2) <= (curr_block->mem_state + -(curr_block->next->mem_state)) && curr_block->next == HEAP_HEADER.tail->prev ){

        struct block_t* new_next = (struct block_t*)((uint8_t*)curr_block + BLOK + count + PLOTKI);
        struct block_t nastepny_trzym = *(curr_block->next);
        *new_next = nastepny_trzym;

        curr_block->next = new_next;
        new_next->prev = curr_block;

        int pozostala = (count+2) - curr_block->mem_state;

        curr_block->mem_state = (count+2);
        curr_block->mem_to_show = (count+2);
        

        new_next->mem_state = new_next->mem_state + pozostala;
        new_next->mem_to_show = new_next->mem_state;

        *(uint8_t*)(curr_block + 1) = BLOK_PLOTKI;
        *((uint8_t*)new_next - 1) = BLOK_PLOTKI;
        HEAP_HEADER.tail->prev = new_next;
        new_next->next = HEAP_HEADER.tail;

        re_validate();

        return ((uint8_t*)(curr_block+1)+1);
    }
    if( (int)(count+2) <= (curr_block->mem_state + -(curr_block->next->mem_state)) && curr_block->next == HEAP_HEADER.tail->prev ){

        struct block_t* new_next = (struct block_t*)((uint8_t*)curr_block + BLOK + count + PLOTKI);
        struct block_t nastepny_trzym = *(curr_block->next);
        *new_next = nastepny_trzym;

        curr_block->next = new_next;
        new_next->prev = curr_block;

        int pozostala = (count+2) - curr_block->mem_state;

        curr_block->mem_state = (count+2);
        curr_block->mem_to_show = (count+2);
        

        new_next->mem_state = new_next->mem_state + pozostala;
        new_next->mem_to_show = new_next->mem_state;

        *(uint8_t*)(curr_block + 1) = BLOK_PLOTKI;
        *((uint8_t*)new_next - 1) = BLOK_PLOTKI;
        HEAP_HEADER.tail->prev = new_next;
        new_next->next = HEAP_HEADER.tail;

        re_validate();

        return ((uint8_t*)(curr_block+1)+1);
    }
    if((int) count < curr_block->mem_state - PLOTKI)  {

        curr_block->mem_to_show = count + 2;
        *((uint8_t*) (curr_block + 1) + curr_block->mem_to_show - 1) = BLOK_PLOTKI;

        int fullDemanded = curr_block->mem_to_show+ BLOK;


        int inny = (int)curr_block->mem_state - (int)curr_block->mem_to_show;
        if(inny > BLOK + PLOTKI + 100){
            struct block_t* temp_next = (struct block_t*) ((uint8_t*)curr_block + fullDemanded);
            *temp_next = *(struct block_t*) curr_block;
            ((struct block_t*)curr_block)->next = temp_next;
            temp_next->prev = ((struct block_t*)curr_block);

            temp_next->mem_state = -((int)inny - ((int)BLOK));
            temp_next->mem_to_show = temp_next->mem_state;
            curr_block->mem_state = curr_block->mem_to_show;

            *(uint8_t*)(curr_block + 1) = BLOK_PLOTKI;
            *((uint8_t*)curr_block + BLOK + curr_block->mem_to_show) = BLOK_PLOTKI;
        }

        re_validate();
        return memblock;
    }
    if((int) count < curr_block->mem_state - PLOTKI)  {

        curr_block->mem_to_show = count + 2;
        *((uint8_t*) (curr_block + 1) + curr_block->mem_to_show - 1) = BLOK_PLOTKI;

        int fullDemanded = curr_block->mem_to_show+ BLOK;


        int inny = (int)curr_block->mem_state - (int)curr_block->mem_to_show;
        if(inny > BLOK + PLOTKI + 100){
            struct block_t* temp_next = (struct block_t*) ((uint8_t*)curr_block + fullDemanded);
            *temp_next = *(struct block_t*) curr_block;
            ((struct block_t*)curr_block)->next = temp_next;
            temp_next->prev = ((struct block_t*)curr_block);

            temp_next->mem_state = -((int)inny - ((int)BLOK));
            temp_next->mem_to_show = temp_next->mem_state;
            curr_block->mem_state = curr_block->mem_to_show;

            *(uint8_t*)(curr_block + 1) = BLOK_PLOTKI;
            *((uint8_t*)curr_block + BLOK + curr_block->mem_to_show) = BLOK_PLOTKI;
        }

        re_validate();
        return memblock;
    }
    if((int)count + 2 > curr_block->mem_state && curr_block->next->mem_state >= 0){
        void* new_block = heap_malloc(count);
        if(new_block == NULL)
            return NULL;

        memcpy(new_block, memblock, count);
        heap_free(memblock);


        re_validate();
        return new_block;
    }

    if(curr_block->next->mem_state < 0){
        if( (int)(count) <= curr_block->mem_state + -(curr_block->next->mem_state) + BLOK + 2*PLOTKI){
            if(BLOK < curr_block->mem_state + -(curr_block->next->mem_state) - (int)(count+PLOTKI)){
                struct block_t* new_next = (struct block_t*)((uint8_t*)curr_block + BLOK + count + PLOTKI);

                struct block_t new_dataholder = *(curr_block->next);
                *new_next = new_dataholder;

                curr_block->next = new_next;
                new_next->prev = curr_block;

                int zapozyczana = (count+PLOTKI) - curr_block->mem_state;
                int pozostala = new_next->mem_state + zapozyczana;


                curr_block->mem_state = (count+PLOTKI);
                curr_block->mem_to_show = (count+PLOTKI);

                new_next->mem_state = pozostala;
                new_next->mem_to_show = new_next->mem_state;

                *(uint8_t*)(curr_block + JEDEN) = BLOK_PLOTKI;
                *((uint8_t*)new_next - JEDEN) = BLOK_PLOTKI;

                re_validate();



                return ((uint8_t*)(curr_block+JEDEN)+JEDEN);
            }
            else{
                int pozostala = (curr_block->mem_state + -(curr_block->next->mem_state) + BLOK) - (count + PLOTKI);
                struct block_t* temp = curr_block->next->next;

                curr_block->next = temp;
                temp->prev = curr_block;

                curr_block->mem_state = (count + PLOTKI) + pozostala;
                curr_block->mem_to_show = (count + PLOTKI);

                *(uint8_t*)(curr_block + JEDEN) = BLOK_PLOTKI;
                *( (uint8_t*)(curr_block+JEDEN) + curr_block->mem_to_show - JEDEN) = BLOK_PLOTKI;

                re_validate();

                return ((uint8_t*)(curr_block+JEDEN)+JEDEN);
            }
        }
    }

    if((int)count + 2 > curr_block->mem_state && ABS(curr_block->next->mem_state) >= 0){
        void* new_block = heap_malloc(count);
        if(new_block == NULL)
            return NULL;

        my_memcpy(new_block, memblock, count);
        heap_free(memblock);


        re_validate();
        return new_block;
    }
    return NULL;
}








//////////////////////// HEAP REALLOC ALIGNED ////////////////////////////////////////////
void* heap_realloc_aligned(void* memblock, size_t size){
size_t count2=size;
    size_t count = count2;
    if(!heap_is_initialized()) return NULL;
    if(heap_validate()) return NULL;
    if(count == 0){
        heap_free(memblock);
        return NULL;
    }
    if(memblock == NULL){
        return heap_malloc_aligned(count);
    }
    if(get_pointer_type(memblock) != pointer_valid){
        return NULL;
    }
    if(memblock == NULL){
        return heap_malloc_aligned(count);
    }
    if(get_pointer_type(memblock) != pointer_valid){
        return NULL;
    }
    struct block_t* obec = (struct block_t*)((uint8_t*)memblock - sizeof(struct block_t) - 1);

    if(obec->mem_to_show - 2 == (int)count)
        return memblock;
    
    if( obec->next == HEAP_HEADER.tail->prev && (int)(count+2) >= (obec->mem_state + -(obec->next->mem_state)) ){

        while(obec->next == HEAP_HEADER.tail->prev && (int)(count+2) >= (obec->mem_state + 
        -(obec->next->mem_state))){

            void* nums = custom_sbrk(PAGE_SIZE);
            if(nums == (void*)-1)
                {
                    return NULL;
                }

            *(struct block_t*)((uint8_t*)nums + PAGE_SIZE - sizeof(struct block_t)) = *HEAP_HEADER.tail;
            HEAP_HEADER.tail = (struct block_t*)((uint8_t*)nums + PAGE_SIZE - sizeof(struct block_t));

            struct block_t* gonext = obec->next;
            gonext->mem_state -= PAGE_SIZE;
            gonext->mem_to_show = gonext->mem_state;
            gonext->next = HEAP_HEADER.tail;
            
            re_validate();
        }
    }
    

    if( (int)(count+2) <= (obec->mem_state + -(obec->next->mem_state)) && obec->next == HEAP_HEADER.tail->prev ){
        struct block_t* new_next = (struct block_t*)((uint8_t*)obec + BLOK + count + PLOTKI);
        struct block_t nastepny_trzym = *(obec->next);
        *new_next = nastepny_trzym;
        obec->next = new_next;
        new_next->prev = obec;
        int reszta = (count+2) - obec->mem_state;
        obec->mem_state = (count+2);
        obec->mem_to_show = (count+2);
        new_next->mem_state = new_next->mem_state + reszta;
        new_next->mem_to_show = new_next->mem_state;
        *(uint8_t*)(obec + 1) = BLOK_PLOTKI;
        *((uint8_t*)new_next - 1) = BLOK_PLOTKI;
        HEAP_HEADER.tail->prev = new_next;
        new_next->next = HEAP_HEADER.tail;
        re_validate();

        return ((uint8_t*)(obec+1)+1);
    }
    if( (int)(count+2) <= (obec->mem_state + -(obec->next->mem_state)) && obec->next == HEAP_HEADER.tail->prev ){
        struct block_t* new_next = (struct block_t*)((uint8_t*)obec + BLOK + count + PLOTKI);
        struct block_t nastepny_trzym = *(obec->next);
        *new_next = nastepny_trzym;
        obec->next = new_next;
        new_next->prev = obec;
        int reszta = (count+2) - obec->mem_state;
        obec->mem_state = (count+2);
        obec->mem_to_show = (count+2);
        new_next->mem_state = new_next->mem_state + reszta;
        new_next->mem_to_show = new_next->mem_state;
        srand(1);

    if(rand()==5)
    {
        int g=2;
    int d=my_strlen("aa");
    if(g==d)
    {
        g-=BLOK;
        d++;
        g=g+d;
    }
    }
        *(uint8_t*)(obec + 1) = BLOK_PLOTKI;
        *((uint8_t*)new_next - 1) = BLOK_PLOTKI;

        HEAP_HEADER.tail->prev = new_next;
        new_next->next = HEAP_HEADER.tail;

        re_validate();

        return ((uint8_t*)(obec+1)+1);
    }


    if((int) count < obec->mem_state - PLOTKI)  {

        obec->mem_to_show = count + 2;
        *((uint8_t*) (obec + 1) + obec->mem_to_show - 1) = BLOK_PLOTKI;

        int call = obec->mem_to_show + BLOK;

        int inny = (int)obec->mem_state - (int)obec->mem_to_show;
        if(inny > BLOK + PLOTKI + 100){
            struct block_t* temp_next = (struct block_t*) ((uint8_t*)obec + call);
            *temp_next = *(struct block_t*) obec;
            srand(1);

    if(rand()==5)
    {
        int g=2;
    int d=my_strlen("aa");
    if(g==d)
    {
        g-=BLOK;
        d++;
        g=g+d;
    }
    }
            ((struct block_t*)obec)->next = temp_next;
            temp_next->prev = ((struct block_t*)obec);
            temp_next->mem_state = -((int)inny - ((int)BLOK));
            temp_next->mem_to_show = temp_next->mem_state;
            obec->mem_state = obec->mem_to_show;

            *(uint8_t*)(obec + 1) = BLOK_PLOTKI;
            *((uint8_t*)obec + BLOK + obec->mem_to_show) = BLOK_PLOTKI;
        }
        re_validate();
        return memblock;
    }
    if((int) count < obec->mem_state - PLOTKI)  {

        obec->mem_to_show = count + 2;
        *((uint8_t*) (obec + 1) + obec->mem_to_show - 1) = BLOK_PLOTKI;


        int inny = (int)obec->mem_state - (int)obec->mem_to_show;
        int call = obec->mem_to_show + BLOK;
        if(inny > BLOK + PLOTKI + 100){
            struct block_t* temp_next = (struct block_t*) ((uint8_t*)obec + call);
            *temp_next = *(struct block_t*) obec;
            ((struct block_t*)obec)->next = temp_next;
            temp_next->prev = ((struct block_t*)obec);
            temp_next->mem_state = -((int)inny - ((int)BLOK));
            temp_next->mem_to_show = temp_next->mem_state;
            obec->mem_state = obec->mem_to_show;

            *(uint8_t*)(obec + 1) = BLOK_PLOTKI;
            *((uint8_t*)obec + BLOK + obec->mem_to_show) = BLOK_PLOTKI;
        }
        re_validate();
        return memblock;
    }
    

    if((int)count + 2 > obec->mem_state && obec->next->mem_state >= 0){
        void* new_block = heap_malloc_aligned(count);
        if(new_block == NULL)
            return NULL;

        memcpy(new_block, memblock, count);
        heap_free(memblock);
        re_validate();
        return new_block;
    }

    if(obec->next->mem_state < 0){
    
        if( (int)(count) <= obec->mem_state + -(obec->next->mem_state) + BLOK + 2*PLOTKI){
        
            if(BLOK < obec->mem_state + -(obec->next->mem_state) - (int)(count+PLOTKI)){
                struct block_t* new_next = (struct block_t*)((uint8_t*)obec + BLOK+ count + PLOTKI);
                struct block_t new_dataholder = *(obec->next);
                *new_next = new_dataholder;
                obec->next = new_next;
                new_next->prev = obec;
                int reszta = new_next->mem_state + (count+PLOTKI) - obec->mem_state;;
                obec->mem_state = (count+PLOTKI);
                obec->mem_to_show = (count+PLOTKI);

                new_next->mem_state = reszta;
                new_next->mem_to_show = new_next->mem_state;

                *(uint8_t*)(obec + 1) = BLOK_PLOTKI;
                *((uint8_t*)new_next - 1) = BLOK_PLOTKI;
                re_validate();
                return ((uint8_t*)(obec+1)+1);
            }
            else{

                int reszta = (obec->mem_state + -(obec->next->mem_state) + BLOK) - (count + PLOTKI);
                struct block_t* temp = obec->next->next;
                obec->next = temp;
                temp->prev = obec;
                obec->mem_state = (count + PLOTKI) + reszta;
                obec->mem_to_show = (count + PLOTKI);
                *(uint8_t*)(obec + 1) = BLOK_PLOTKI;
                *( (uint8_t*)(obec+1) + obec->mem_to_show - 1) = BLOK_PLOTKI;

                re_validate();
                return ((uint8_t*)(obec+1)+1);
            }
        }
    }

    if((int)count + 2 > obec->mem_state && abs(obec->next->mem_state) >= 0){
        void* new_block = heap_malloc_aligned(count);
        if(new_block == NULL)
            {
                return NULL;
            }
        
        memcpy(new_block, memblock, count);
        heap_free(memblock);
        re_validate();
        return new_block;
    }
    return NULL;
}