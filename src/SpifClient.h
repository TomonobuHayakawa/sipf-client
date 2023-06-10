/*
 *  SpifClient.h
 *  Author T.Hayakawa
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  This sketch connects to a website via LTE. Specifically,
 *  this example downloads the URL "http://arduino.tips/asciilogo.txt" and
 *  prints it to the Serial monitor.
 */


// libraries
#include <String.h>
#include <WString.h>
#include <LTE.h>
#include <ArduinoHttpClient.h>

#include <File.h>

// APN name
#define APP_LTE_APN "sakura" // replace your APN

/* APN authentication settings
 * Ignore these parameters when setting LTE_NET_AUTHTYPE_NONE.
 */
#define APP_LTE_USER_NAME "" // replace with your username
#define APP_LTE_PASSWORD  "" // replace with your password

// APN IP type
#define APP_LTE_IP_TYPE (LTE_NET_IPTYPE_V4V6) // IP : IPv4v6
// #define APP_LTE_IP_TYPE (LTE_NET_IPTYPE_V4) // IP : IPv4
// #define APP_LTE_IP_TYPE (LTE_NET_IPTYPE_V6) // IP : IPv6

// APN authentication type
#define APP_LTE_AUTH_TYPE (LTE_NET_AUTHTYPE_CHAP) // Authentication : CHAP
// #define APP_LTE_AUTH_TYPE (LTE_NET_AUTHTYPE_PAP) // Authentication : PAP
// #define APP_LTE_AUTH_TYPE (LTE_NET_AUTHTYPE_NONE) // Authentication : NONE

/* RAT to use
 * Refer to the cellular carriers information
 * to find out which RAT your SIM supports.
 * The RAT set on the modem can be checked with LTEModemVerification::getRAT().
 */

#define APP_LTE_RAT (LTE_NET_RAT_CATM) // RAT : LTE-M (LTE Cat-M1)
// #define APP_LTE_RAT (LTE_NET_RAT_NBIOT) // RAT : NB-IoT


#define OBJ_MAX_CNT (255)

#define OBJ_HEADER_SIZE 12
#define MAX_PAYLOAD_SIZE 1024

#define OBJ_TYPE_UINT8 0x00
#define OBJ_TYPE_INT8 0x01
#define OBJ_TYPE_UINT16 0x02
#define OBJ_TYPE_INT16 0x03
#define OBJ_TYPE_UINT32 0x04
#define OBJ_TYPE_INT32 0x05
#define OBJ_TYPE_UINT64 0x06
#define OBJ_TYPE_INT64 0x07
#define OBJ_TYPE_FLOAT32 0x08
#define OBJ_TYPE_FLOAT64 0x09
#define OBJ_TYPE_BIN_BASE64 0x10
#define OBJ_TYPE_BIN 0x10
#define OBJ_TYPE_STR_UTF8 0x20

#define OBJ_SIZE_UINT8   0x01
#define OBJ_SIZE_INT8    0x01
#define OBJ_SIZE_UINT16  0x02
#define OBJ_SIZE_INT16   0x02
#define OBJ_SIZE_UINT32  0x04
#define OBJ_SIZE_INT32   0x04
#define OBJ_SIZE_UINT64  0x08
#define OBJ_SIZE_INT64   0x08
#define OBJ_SIZE_FLOAT32 0x04
#define OBJ_SIZE_FLOAT64 0x08

typedef enum {
    OBJECTS_UP = 0x00,
    OBJECTS_UP_RETRY = 0x01,
    OBJID_NOTIFICATION = 0x02,

    OBJECTS_DOWN_REQUEST = 0x11,
    OBJECTS_DOWN = 0x12,
    /*
    OBJID_REACH_INQUIRY = 0xff,
    OBJID_REACH_RESULT = 0xff,
    OBJID_REACH_NOTIFICATION = 0xff,
    */
    OBJ_COMMAND_ERR = 0xff,
} SipfObjectCommandType;

typedef struct
{
    SipfObjectCommandType command_type;
    uint64_t command_time;
    uint8_t option_flag;
    uint16_t command_payload_size;
} SipfObjectCommandHeader;

typedef struct
{
    uint8_t obj_type;
    uint8_t obj_tagid;
    uint8_t value_len;
    uint8_t *value;
} SipfObjectObject;

typedef struct
{
    uint8_t obj_qty;
    SipfObjectObject obj;
} SipfObjectUp;

typedef struct
{
    uint8_t value[16];
} SipfObjectOtid;


const char auth_server[] = "auth.sipf.iot.sakura.ad.jp";
const char auth_path[]   = "/v0/session_key";

const char data_server[] = "da.sipf.iot.sakura.ad.jp";
const char data_path[]   = "/v0";

const char file_server[] = "file.sipf.iot.sakura.ad.jp";
const char file_path[]   = "/v1/files/";
const char file_comple[]   = "/complete/";

class SpifClient
{

public:

  void begin(LTEClient*, int);
  void begin(LTETLSClient*, int);
  void end();

  bool authorization();

  bool upload(uint64_t, SipfObjectObject*, uint8_t);
  bool receiveResult(int64_t*);

  bool SipfFileRequestUploadURL(File&, String);

private:

  String user;
  String pass;

  int port = 80; // port 80 is the default for HTTP

  LTEClient* client;
  LTETLSClient* tlsclient;

  HttpClient* http_client;
  HttpClient* http_file_client;
  
  uint8_t objectBuffer[OBJ_HEADER_SIZE+MAX_PAYLOAD_SIZE];

  int SipfObjectCreateObjUpPayload(uint8_t *raw_buff, uint16_t sz_raw_buff, SipfObjectObject *objs, uint8_t obj_qty);
  int SipfObjectCreateObjUp(uint8_t* ptr, uint64_t utime, SipfObjectObject *objs, uint8_t obj_qty);

};
