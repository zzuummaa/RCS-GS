#define PACKET_TIMEOUT 500000

char sent = 0;

const byte nBlocksMax = 20;
unsigned int terminalPacketID=0;
unsigned int nBlocks=0;
uint8_t** packet;
byte* blockSize;

#include "rfm23bp.h"

void Setup_Transmitter()
{
  packet = new uint8_t*[nBlocksMax];
  for(byte i=0; i<nBlocksMax; i++)
    packet[i] = new uint8_t[blockSizeMax];
  // Initial contence of packet is not inmortant
  
  blockSize = new byte[nBlocksMax];
  for(byte i=0; i<nBlocksMax; i++)
    blockSize[i] = 0;
}

void FormPacket(const char* data, unsigned int size)
{
  unsigned int bytesToPack = size;
  unsigned int bytesPacked = 0;
  
  // Division into blocks
  //for(unsigned int i = size; i>0; i--)
  while(bytesToPack > 0)
  {
    byte freeBytes = blockSizeMax - blockSize[nBlocks];
    
    if(blockSize[nBlocks] == 0)
    {
      byte* packetIDPtr = (byte*)&terminalPacketID;
      byte* nBlocksPtr = (byte*)&nBlocks;
      
      // Sending terminalPacketID explicitely
      packet[nBlocks][0] = packetIDPtr[0];
      packet[nBlocks][1] = packetIDPtr[1];
      blockSize[nBlocks] += 2;
        
      // Sending block number
      //packet[nBlocks][blockSize[nBlocks]] = nBlocks;
      packet[nBlocks][2] = nBlocksPtr[0];
      packet[nBlocks][3] = nBlocksPtr[1];
      blockSize[nBlocks] += 2;
    }
    
    if(bytesToPack <= freeBytes)
    {
      for(byte i = 0; i < bytesToPack; i++)
        packet[nBlocks][blockSize[nBlocks] + i] = data[bytesPacked + i];
        
      blockSize[nBlocks] += bytesToPack;
      return;
    }
    else
    {
      for(byte i = 0; i < freeBytes; i++)
        packet[nBlocks][blockSize[nBlocks] + i] = data[bytesPacked + i];
        
      blockSize[nBlocks] = blockSizeMax;
      nBlocks++;
      bytesPacked += freeBytes;
      bytesToPack -= freeBytes;
    }
  }
}

void RenewPacket()
{
  nBlocks = 0;
  
  for(byte i=0; i<nBlocksMax; i++)
    blockSize[i] = 0;
}

void PrintPacket()
{
  // Print only the first block just for debugging...
  Serial.print("Contence of the packet #");
  Serial.println(terminalPacketID);
  for(byte i=0; i<=nBlocks; i++)
  {
    Serial.print("Block ");
    Serial.print(i);
    Serial.print(". Size: ");
    Serial.println(blockSize[i]);
    for(byte j=0; j<blockSize[i]; j++)
    {
      Serial.print(packet[i][j]);
//      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
}



bool SendPacket(byte retries)
{
  for(byte j=0; j<=nBlocks; j++)
  {
    rfm23bp_transmit(packet[j],blockSize[j]);
    //delayMicroseconds(800);
    //delay(100);
    unsigned long start_time = millis();
    while((sent==0) && ((millis() - start_time)<=PACKET_TIMEOUT))
    {
      ;
    }
    sent = 0;
  }
  RenewPacket();
  return true;
}

void SendCommand(char* command, unsigned int size) 
{
  unsigned int cmdLength = size + 2;
  unsigned int seconds = millis() / 1000;
  FormPacket("time", 4); 
  FormPacket((char*)&seconds, 2); // We are sending 4-byte time format now...
  FormPacket("pType", 5);
  FormPacket("TXT", 3);
  FormPacket((char*)&cmdLength, 2);
  FormPacket(command, size);
  FormPacket("\n\r", 2);
  FormPacket(EP, EPLength);
  SendPacket(10);
  terminalPacketID++;
  RenewPacket();
  rfm23bp_enable_rx();
  Serial.println("sent");
}


