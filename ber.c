#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define uint8 unsigned char
typedef struct
{
uint8*  tag;
uint8*  length;
uint8*  value;
uint8*  tlvBuffer;
uint8  tagLen;
uint8  lengthLen;
uint8  valueLen;
uint8  bufferlen;
uint8  nextlevel;
}tlvInfoST;

typedef struct tlvNode {
    tlvInfoST * tlvInfo;
    struct tlvNode * next;
} tlvNode_t;

static tlvNode_t* tlvheaders;
static tlvNode_t* tlvcurrents;
static uint8* tlvListBuffers;

void byteArrayToHexStringWithZero(uint8* byteArray, uint8 length, uint8* outStr)
{
    uint8 i = 0;
    char hex[3] = {0};
    for (i = 0; i < length; i++)
    {
        sprintf(hex, "%02X", byteArray[i]);
        strcat(outStr, hex);
    }
}
uint8* getTlvListBuffer(void)
{
    return tlvListBuffers;
}
void freeTlvListBuffer(void)
{
    free(tlvListBuffers);
    tlvListBuffers = NULL;
}
uint8 initTlvList(void)
{
    if((tlvheaders = (tlvNode_t*)calloc(1, sizeof(tlvNode_t))) == NULL)
    {
       return 0;
    }
    tlvheaders->tlvInfo = NULL;
    tlvheaders->next = NULL;
    tlvcurrents = tlvheaders;
    return 1;
}
uint8 initTlvListBuffer(void)
{
    return 1;
}
void printAllOfTlvTagValue(void)
{
    uint8 tagStr[5];
    uint8 valueStr[1000];
    uint8 i = 0;
    for(tlvcurrents = tlvheaders; tlvcurrents != NULL; tlvcurrents = tlvcurrents->next)
    {
        memset(tagStr, 0x00, sizeof(tagStr));
        memset(valueStr, 0x00, sizeof(valueStr));
        byteArrayToHexStringWithZero(tlvcurrents->tlvInfo->tag, tlvcurrents->tlvInfo->tagLen, tagStr);
        byteArrayToHexStringWithZero(tlvcurrents->tlvInfo->value, tlvcurrents->tlvInfo->valueLen, valueStr);
        for(i = 0; i < tlvcurrents->tlvInfo->nextlevel; i++)
            printf("\n");

        printf("%s:-->Tag \n %s--> value\n", tagStr, valueStr);
    }
}
uint8 getTagLenFromBufferList(uint8* tagStartBuffer)
{
    uint8* tag;
    uint8 maskFirstSubsequentByte = 0x1F;
    uint8 maskAnotherTagByte = 0x80;
    uint8 lastByte = 0;
    uint8 i = 0;
    if((tag = (uint8*)calloc(1, sizeof(uint8))) == NULL)
    {
       return 0;
    }
    memcpy(tag, tagStartBuffer, i + 1);
    if((tag[i] & maskFirstSubsequentByte) == maskFirstSubsequentByte)
    {
        i++;
        if((tag = (uint8*)realloc(tag, (i + 1) * sizeof(uint8))) == NULL)
        {
            return 0;
        }
        memcpy(tag, tagStartBuffer, i + 1);
        while(lastByte == 0)
        {
            if((tag[i] & maskAnotherTagByte) == maskAnotherTagByte)
            {
                i++;
                if((tag = (uint8*)realloc(tag, (i + 1) * sizeof(uint8))) == NULL)
                {
                   return 0;
                }
                memcpy(tag, tagStartBuffer, i + 1);
            }
            else
                lastByte = 1;
        }
    }
    free(tag);
    return i + 1;
}
 uint8 getlengthLenFromList(uint8 firstByteOfLength)
 {
    uint8 lengthLen;
    uint8 maskFirstMsbBit = 0x80;
    if ((firstByteOfLength & maskFirstMsbBit) == maskFirstMsbBit)
        lengthLen = (firstByteOfLength & ~(maskFirstMsbBit)) + 1;
    else
        lengthLen = 1;
    return lengthLen;
 }
 uint8 getValueLenFromLength(uint8* length)
 {
     uint8 lengthLen = getlengthLenFromList(length[0]);
     uint8 valueLen = 0;
     uint8 i;
     if(lengthLen == 1)
     {
        valueLen = length[0];
     }
     else if(lengthLen > 1)
     {
         for(i = 1; i < lengthLen; i++)
         {
              valueLen |= length[i] << (8 * (lengthLen - i - 1));
         }
     }
    return valueLen;
 }
uint8 getNextTlvFromList(uint8* tlvListBufferPtr, uint8 nextlevel)
{
tlvNode_t* tlvPrevious;
tlvInfoST* tlvInfo;
uint8 offset = 0;
uint8 bufferlen;
uint8 maskCheckNested = 0x20;
if(tlvcurrents->tlvInfo != NULL)
{
    if((tlvcurrents->next = (tlvNode_t*)calloc(1, sizeof(tlvNode_t))) == NULL)
    {
        return 0;
    }
    tlvPrevious = tlvcurrents;
    tlvcurrents = tlvcurrents->next;
    tlvcurrents->next = NULL;
}
if((tlvcurrents->tlvInfo = (tlvInfoST*)calloc(1, sizeof(tlvInfoST))) == NULL)
{
    free(tlvcurrents);
    tlvcurrents = tlvPrevious;
    tlvcurrents->next =NULL;
    return 0;
}
tlvInfo = tlvcurrents->tlvInfo;
tlvInfo->nextlevel = nextlevel;
tlvInfo->tagLen = getTagLenFromBufferList(tlvListBufferPtr);
if((tlvInfo->tag = (uint8*)calloc(tlvInfo->tagLen, sizeof(uint8))) == NULL)
{
    free(tlvInfo);
    free(tlvcurrents);
    tlvcurrents = tlvPrevious;
    tlvcurrents->next =NULL;
    return 0;
}
memcpy(tlvInfo->tag, tlvListBufferPtr + offset, tlvInfo->tagLen);
offset += tlvInfo->tagLen;
tlvInfo->lengthLen = getlengthLenFromList(*(uint8*)(tlvListBufferPtr + offset));
if((tlvInfo->length = (uint8*)calloc(tlvInfo->lengthLen, sizeof(uint8))) == NULL)
{
    free(tlvInfo->tag);
    free(tlvInfo);
    free(tlvcurrents);
    tlvcurrents = tlvPrevious;
    tlvcurrents->next =NULL;
    return 0;
}
memcpy(tlvInfo->length, tlvListBufferPtr + offset, tlvInfo->lengthLen);
tlvInfo->valueLen = getValueLenFromLength(tlvInfo->length);
if((tlvInfo->value = (uint8*)calloc(tlvInfo->valueLen, sizeof(uint8))) == NULL)
{
    free(tlvInfo->tag);
    free(tlvInfo->length);
    free(tlvInfo);
    free(tlvcurrents);
    tlvcurrents = tlvPrevious;
    tlvcurrents->next =NULL;
    return 0;
}
offset += tlvInfo->lengthLen;
memcpy(tlvInfo->value, tlvListBufferPtr + offset, tlvInfo->valueLen);
tlvInfo->bufferlen =  tlvInfo->tagLen + tlvInfo->lengthLen + tlvInfo->valueLen;
bufferlen =  tlvInfo->bufferlen;
if(tlvInfo->tag[0] & maskCheckNested)
{
    nextlevel++;
    offset = tlvInfo->tagLen + tlvInfo->lengthLen;
    while(offset < bufferlen)
    {
        offset +=  getNextTlvFromList(tlvListBufferPtr + offset, nextlevel);
    }
}
return bufferlen;
}
uint8 tlvListParse(uint8* tlvListBuffer,uint8  tlvListBufferLength)
{
    uint8* tlvListBufferPtr;
    uint8 bufferlen = 0;
    tlvListBufferPtr = tlvListBuffer;
    while(tlvListBufferPtr - tlvListBuffer <  tlvListBufferLength){
        bufferlen = getNextTlvFromList(tlvListBufferPtr, 0);
        tlvListBufferPtr = (uint8*)(tlvListBufferPtr + bufferlen);
    }
    return 1;
}
int hex2bin(const char* hexstr, uint8 *bytes)
{
    int count = 0;
    size_t len = strlen(hexstr);
    if (len % 2)
        return 0;
    size_t final_len = len / 2;
    for (size_t i=0, j=0; j<final_len; i+=2, j++)
        bytes[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i+1] % 32 + 9) % 25;
        count++;
    bytes[final_len] = '\0';
    return count;
}
int main()
{
    char hexStr[] = "6F3E8407A0000000041010A533500A4D617374657243617264870101BF0C215F501E506179687564646C6520546573742043617264202D2053756363657373209000";
    int hexStrLen = strlen(hexStr);
    uint8 bytes[hexStrLen/ 2];
	uint8 bytesLength = hexStrLen / 2;
    hex2bin(hexStr, bytes);
	initTlvList();
	tlvListParse(bytes, bytesLength);
    printAllOfTlvTagValue();
    return 0;
}

