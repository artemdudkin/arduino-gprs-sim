#pragma once

typedef unsigned char __FlashStringHelper;

typedef struct {
	const char *cmd;
	const char *resp;
} FlowItem;
#define FLOW_SIZE 32

#define SS_BUF_SIZE 64

class NeoSWSerial
{
  public:
    NeoSWSerial(int pinRx, int pinTx);
    
    void begin(int rate);
    
    void listen();
    char read();
    int available();
    
    void print( const char *msg);
    void print( const __FlashStringHelper *msg);
    void print( const char c);
    void print( int msg);
    void print( long msg);
    
    void println( const char *msg);
    void println( const __FlashStringHelper *msg);
    void println( const char c);
    void println( int msg);
    void println( long msg);

    //all below is fixture methods (include static fields and methods)
    //------------------------------------------------------
    static void flow_add(const char*, const char*);
    static void flow_clear();
  private:
    int flow_current;
    int flow_next;
    int flow_pos;
	char buf[SS_BUF_SIZE];
    int  buf_length;
    int  buf_pos;
    bool buf_reading;  
    
    void check_available();
    void check_read();
    void check_buf_reading();
  
    static FlowItem   *flow;
    static int         flow_length;
    static const char *flow_err[];
};

//FlowItem *SoftwareSerial::flow = malloc( FLOW_SIZE * sizeof(FlowItem));
