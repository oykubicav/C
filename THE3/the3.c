#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "the3.h"

/*
    Create a cache by reading inputs from stdin
*/
char* copy_token(char* token) {
    size_t len = strlen(token);
    char* copy = (char*)malloc(len + 1);
    strcpy(copy, token);
    return copy;
}


void addToCache(Cache* cache, char* mediaName, int mediaSize, DomainFreqList* domainFreqList) {
    CacheNode* newNode = (CacheNode*)malloc(sizeof(CacheNode));
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->media.name = copy_token(mediaName);
    newNode->media.size = mediaSize;
    newNode->domainFreqList = domainFreqList;

    if (cache->head == NULL) {
        cache->head = newNode;
        cache->tail = newNode;
    } else {
        newNode->prev = cache->tail;
        cache->tail->next = newNode;
        cache->tail = newNode;
    }

    cache->mediaCount++;
    cache->currentSize += mediaSize;

   
}
void deleteCacheNode(Cache* cache, CacheNode* node) {
    DomainFreqNode* temp;
    DomainFreqNode* current;
    if (node == NULL) {
        return;
    }

    if (node->prev == NULL) {
        cache->head = node->next;
    } else {
        node->prev->next = node->next;
    }

    if (node->next == NULL) {
        cache->tail = node->prev;
    } else {
        node->next->prev = node->prev;
    }

    cache->mediaCount--;
    cache->currentSize -= node->media.size;

    free(node->media.name);

    current = node->domainFreqList->head;

    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp->name);
        free(temp);
    }

    free(node->domainFreqList);
    free(node);
}

void insertDomainFreqNode(DomainFreqList* list, char* domainName, int domainFreq) {
    DomainFreqNode* newNode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));
    newNode->name = domainName;
    newNode->freq = domainFreq;
    newNode->prev = NULL;
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        DomainFreqNode* current = list->head;
        DomainFreqNode* prev = NULL;

        while (current != NULL) {
            if (newNode->freq > current->freq || (newNode->freq == current->freq && strcmp(newNode->name, current->name) < 0)) {
                break;
            }
            prev = current;
            current = current->next;
        }

        if (prev == NULL) {
            newNode->next = list->head;
            if (list->head != NULL) {
                list->head->prev = newNode;
            }
            list->head = newNode;
        } else if (current == NULL) {
            newNode->prev = list->tail;
            if (list->tail != NULL) {
                list->tail->next = newNode;
            }
            list->tail = newNode;
        } else {
            newNode->prev = prev;
            newNode->next = current;
            prev->next = newNode;
            current->prev = newNode;
        }
    }
}

Cache* createCache() {
     Media* media;

    DomainFreqNode* domainFreqNode;
    Cache* cache = (Cache*)malloc(sizeof(Cache));
    int cacheSize, mediaCount;
    CacheNode* newNode;
    char* token;
    char line[1000];
    char* domainName;
    int i;
    int domainFreq;
    cache->head = NULL;
    cache->tail = NULL;
    cache->mediaCount = 0;
    cache->currentSize = 0;

    scanf("%d %d\n", &cacheSize, &mediaCount);
    cache->cacheLimit = cacheSize;

   

    for (i = 0; i < mediaCount; i++) {
        newNode = (CacheNode*)malloc(sizeof(CacheNode));
        newNode->prev = NULL;
        newNode->next = NULL;
        newNode->domainFreqList = (DomainFreqList*)malloc(sizeof(DomainFreqList));
        newNode->domainFreqList->head = NULL;
        newNode->domainFreqList->tail = NULL;

        fgets(line, sizeof(line), stdin);
        

        token = strtok(line, " ");
          

        newNode->media.name = copy_token(token);

        token = strtok(NULL, " ");
        newNode->media.size = atoi(token);

        token = strtok(NULL, " ");
        while (token != NULL) {
            domainFreqNode = (DomainFreqNode*)malloc(sizeof(DomainFreqNode));

           domainFreqNode->name = copy_token(token);

            token = strtok(NULL, " ");
            domainFreqNode->freq = atoi(token);


            insertDomainFreqNode(newNode->domainFreqList,domainFreqNode->name ,domainFreqNode->freq );

            token = strtok(NULL, " ");
        }

        

        addToCache(cache, newNode->media.name, newNode->media.size, newNode->domainFreqList);
    }

    return cache;
}

/*
    Prints cache information and its contents
*/
void printCache(Cache* cache) {
    CacheNode* current;
    DomainFreqNode* domainNode;
    printf("-------------- Cache Information --------------\n");
    printf("Cache Limit: %d KB\n", cache->cacheLimit);
    printf("Current Size: %d KB\n", cache->currentSize);
    printf("Media Count: %d\n", cache->mediaCount);

    if (cache->head == NULL) {
        printf("The Cache is Empty\n");
        return;
    }

    printf("Cache Media Info:\n");

     current = cache->head;
    while (current != NULL) {
        printf("\tName: %s\tSize: %d KB\n", current->media.name, current->media.size);
        printf("\t------- Media Domain Frequencies -------\n");

         domainNode = current->domainFreqList->head;
        while (domainNode != NULL) {
            printf("\t\tName: %s\tFreq: %d\n", domainNode->name, domainNode->freq);
            domainNode = domainNode->next;
        }

        printf("\t----------------------------------------\n");
        current = current->next;
    }

    printf("----------------------------------------------\n");
}



/*
    Adds media to the cache
*/
CacheNode* addMediaRequest(Cache* cache, Media media, char* domain) {
    CacheNode* current = cache->head;
    DomainFreqNode* freqnode;
    CacheNode* newNode;

    while (current != NULL) {
        if (strcmp(current->media.name, media.name) == 0) {
       
            if (current != cache->head) {
                current->prev->next = current->next;
                if (current->next != NULL) {
                    current->next->prev = current->prev;
                } else {
                    cache->tail = current->prev;
                }
                current->prev = NULL;
                current->next = cache->head;
                cache->head->prev = current;
                cache->head = current;
            }

            current->media = media;
            freqnode = current->domainFreqList->head;

      
            while (freqnode != NULL) {
                if (strcmp(freqnode->name, domain) == 0) {
                    freqnode->freq++;
                    return cache->head;
                }
                freqnode = freqnode->next;
            }

          
            insertDomainFreqNode(current->domainFreqList,copy_token(domain) , 1);
            return cache->head;
        }

        current = current->next;
    }

    if ((cache->cacheLimit) - (cache->currentSize) >= media.size) {

        CacheNode* newNode = (CacheNode*)malloc(sizeof(CacheNode));
        newNode->prev = NULL;
        newNode->next = cache->head;
        if (cache->head != NULL) {
            cache->head->prev = newNode;
        } else {
            cache->tail = newNode;
        }
        cache->head = newNode;
        newNode->media.name = copy_token(media.name);
        newNode->media.size = media.size;
        newNode->domainFreqList = (DomainFreqList*)malloc(sizeof(DomainFreqList));
        newNode->domainFreqList->head = NULL;
        newNode->domainFreqList->tail = NULL;
        insertDomainFreqNode(newNode->domainFreqList, copy_token(domain), 1);
        cache->mediaCount++;
        cache->currentSize += media.size;
    } else {
    
        while (cache->tail != NULL && (cache->cacheLimit - cache->currentSize) < media.size) {
            CacheNode* tail = cache->tail;
            cache->tail = tail->prev;
            if (tail->prev != NULL) {
                tail->prev->next = NULL;
            } else {
                cache->head = NULL;
            }
            
            deleteCacheNode(cache, tail);
        }
        
         newNode = (CacheNode*)malloc(sizeof(CacheNode));
        newNode->prev = NULL;
        newNode->next = cache->head;
        if (cache->head != NULL) {
            cache->head->prev = newNode;
        } else {
            cache->tail = newNode;
        }
        cache->head = newNode;
        newNode->media = media;
        newNode->domainFreqList = (DomainFreqList*)malloc(sizeof(DomainFreqList));
        newNode->domainFreqList->head = NULL;
        newNode->domainFreqList->tail = NULL;
        insertDomainFreqNode(newNode->domainFreqList,copy_token(domain) , 1);
        cache->mediaCount++;
        cache->currentSize += media.size;
    }

    return cache->head;
}


/*
    Finds the corresponding media's node in the cache
    Return NULL if it is not found
*/
CacheNode* findMedia(Cache* cache, char* name) {
    CacheNode* current = cache->head;

    while (current != NULL) {
        if (strcmp(current->media.name, name) == 0) {
            return current; 
        }
        current = current->next;
    }

    return NULL; }

/*
    Finds a media from its name and deletes it's node from the cache
    Do not do anything if media is not found
*/
void deleteMedia(Cache* cache, char* name) {
    CacheNode* current = cache->head;

    while (current != NULL) {
        if (strcmp(current->media.name, name) == 0) {
            CacheNode* nextNode = current->next;
            deleteCacheNode(cache, current);
            current = nextNode;
        } else {
            current = current->next;
        }
    }
}

