# 1 "Client.c"
# 1 "<eingebaut>"
# 1 "<Kommandozeile>"
# 1 "Client.c"

# 1 "Client.h" 1
# 1 "ClientLib.h" 1



int initClient();

char* getClientName(int handle);

void setClientName(int handle, char* value);

int getClientOutbuffer(int handle);

void setClientOutbuffer(int handle, int value);

int getClientAddressBookSize(int handle);

void setClientAddressBookSize(int handle, int value);

int createClientAddressBookEntry(int handle);

int getClientAddressBookAlias(int handle, int index);

void setClientAddressBookAlias(int handle, int index, int value);

int getClientAddressBookAddress(int handle, int index);

void setClientAddressBookAddress(int handle, int index, int value);


int getClientAutoResponse(int handle);

void setClientAutoResponse(int handle, int value);

int getClientPrivateKey(int handle);

void setClientPrivateKey(int handle, int value);

int getClientKeyringSize(int handle);

int createClientKeyringEntry(int handle);

int getClientKeyringUser(int handle, int index);

void setClientKeyringUser(int handle, int index, int value);

int getClientKeyringPublicKey(int handle, int index);

void setClientKeyringPublicKey(int handle, int index, int value);

int getClientForwardReceiver(int handle);

void setClientForwardReceiver(int handle, int value);

int getClientId(int handle);

void setClientId(int handle, int value);

int findPublicKey(int handle, int userid);

int findClientAddressBookAlias(int handle, int userid);
# 2 "Client.h" 2

# 1 "Email.h" 1
# 1 "featureselect.h" 1






int __SELECTED_FEATURE_Base;

int __SELECTED_FEATURE_Keys;

int __SELECTED_FEATURE_Encrypt;

int __SELECTED_FEATURE_AutoResponder;

int __SELECTED_FEATURE_AddressBook;

int __SELECTED_FEATURE_Sign;

int __SELECTED_FEATURE_Forward;

int __SELECTED_FEATURE_Verify;

int __SELECTED_FEATURE_Decrypt;

int __GUIDSL_ROOT_PRODUCTION;


int select_one();
void select_features();
void select_helpers();
int valid_product();
# 2 "Email.h" 2

# 1 "EmailLib.h" 1



int initEmail();

int getEmailId(int handle);

void setEmailId(int handle, int value);

int getEmailFrom(int handle);

void setEmailFrom(int handle, int value);

int getEmailTo(int handle);

void setEmailTo(int handle, int value);

char* getEmailSubject(int handle);

void setEmailSubject(int handle, char* value);

char* getEmailBody(int handle);

void setEmailBody(int handle, char* value);

int isEncrypted(int handle);

void setEmailIsEncrypted(int handle, int value);

int getEmailEncryptionKey(int handle);

void setEmailEncryptionKey(int handle, int value);

int isSigned(int handle);

void setEmailIsSigned(int handle, int value);

int getEmailSignKey(int handle);

void setEmailSignKey(int handle, int value);

int isVerified(int handle);

void setEmailIsSignatureVerified(int handle, int value);
# 4 "Email.h" 2


void printMail (int msg);


int isReadable (int msg);


int createEmail (int from, int to);


int cloneEmail(int msg);
# 4 "Client.h" 2

# 1 "EmailLib.h" 1



int initEmail();

int getEmailId(int handle);

void setEmailId(int handle, int value);

int getEmailFrom(int handle);

void setEmailFrom(int handle, int value);

int getEmailTo(int handle);

void setEmailTo(int handle, int value);

char* getEmailSubject(int handle);

void setEmailSubject(int handle, char* value);

char* getEmailBody(int handle);

void setEmailBody(int handle, char* value);

int isEncrypted(int handle);

void setEmailIsEncrypted(int handle, int value);

int getEmailEncryptionKey(int handle);

void setEmailEncryptionKey(int handle, int value);

int isSigned(int handle);

void setEmailIsSigned(int handle, int value);

int getEmailSignKey(int handle);

void setEmailSignKey(int handle, int value);

int isVerified(int handle);

void setEmailIsSignatureVerified(int handle, int value);
# 6 "Client.h" 2
# 14 "Client.h"
void queue (int client, int msg);


int is_queue_empty ();

int get_queued_client ();

int get_queued_email ();


void mail (int client, int msg);

void outgoing (int client, int msg);

void deliver (int client, int msg);

void incoming (int client, int msg);

int createClient(char *name);


void sendEmail (int sender, int receiver);



int
isKeyPairValid (int publicKey, int privateKey);


void
generateKeyPair (int client, int seed);
void autoRespond (int client, int msg);
void sendToAddressBook (int client, int msg);

void sign (int client, int msg);

void forward (int client, int msg);

void verify (int client, int msg);
# 3 "Client.c" 2


int queue_empty = 1;


int queued_message;


int queued_client;



void
mail (int client, int msg)
{

  incoming (getEmailTo(msg), msg);
}



void
outgoing__before__Encrypt (int client, int msg)
{
  setEmailFrom(msg, getClientId(client));
  mail(client, msg);
}



void
outgoing__role__Encrypt (int client, int msg)
{


    int receiver = getEmailTo(msg);
    int pubkey = findPublicKey(client, receiver);
    if (pubkey) {
        setEmailEncryptionKey(msg, pubkey);
        setEmailIsEncrypted(msg, 1);
    }



  outgoing__before__Encrypt(client, msg);
}
void
outgoing__before__AddressBook(int client, int msg) {
    if (__SELECTED_FEATURE_Encrypt) {
        outgoing__role__Encrypt(client, msg);
    } else {
        outgoing__before__Encrypt(client, msg);
    }
}


void
outgoing__role__AddressBook (int client, int msg)
{

    int size = getClientAddressBookSize(client);

    if (size) {
        sendToAddressBook(client, msg);

        int receiver = getEmailTo(msg);


        int second = getClientAddressBookAddress(client, 1);
        setEmailTo(msg, second);
        outgoing__before__AddressBook(client, msg);


        setEmailTo(msg, getClientAddressBookAddress(client, 0));
        outgoing__before__AddressBook(client, msg);
    } else {
        outgoing__before__AddressBook(client, msg);
    }

}



void
outgoing__before__Sign(int client, int msg) {
    if (__SELECTED_FEATURE_AddressBook) {
        outgoing__role__AddressBook(client, msg);
    } else {
        outgoing__before__AddressBook(client, msg);
    }
}





void
outgoing__role__Sign (int client, int msg)
{
  sign (client, msg);
  outgoing__before__Sign(client, msg);
}


void
outgoing(int client, int msg) {
    if (__SELECTED_FEATURE_Sign) {
        outgoing__role__Sign(client, msg);
    } else {
        outgoing__before__Sign(client, msg);
    }
}





void
deliver (int client, int msg)
{

}



void
incoming__before__AutoResponder (int client, int msg)
{
  deliver (client, msg);
}



void
incoming__role__AutoResponder (int client, int msg)
{
  incoming__before__AutoResponder(client, msg);
  if (getClientAutoResponse(client)) {
    autoRespond (client, msg);
  }
}


void
incoming__before__Forward(int client, int msg) {
    if (__SELECTED_FEATURE_AutoResponder) {
        incoming__role__AutoResponder(client, msg);
    } else {
        incoming__before__AutoResponder(client, msg);
    }
}




void
incoming__role__Forward (int client, int msg)
{
  incoming__before__Forward(client, msg);
  int fwreceiver = getClientForwardReceiver(client);
  if (fwreceiver) {

    setEmailTo(msg, fwreceiver);
    forward (client, msg);

  }
}


void
incoming__before__Verify(int client, int msg) {
    if (__SELECTED_FEATURE_Forward) {
        incoming__role__Forward(client, msg);
    } else {
        incoming__before__Forward(client, msg);
    }
}




void
incoming__role__Verify (int client, int msg)
{
  verify (client, msg);
  incoming__before__Verify(client, msg);
}


void
incoming__before__Decrypt(int client, int msg) {
    if (__SELECTED_FEATURE_Verify) {
        incoming__role__Verify(client, msg);
    } else {
        incoming__before__Verify(client, msg);
    }
}




void
incoming__role__Decrypt (int client, int msg)
{


  int privkey = getClientPrivateKey(client);
  if (privkey) {

      if (isEncrypted(msg)
          && isKeyPairValid(getEmailEncryptionKey(msg), privkey))
        {
          setEmailIsEncrypted(msg, 0);
          setEmailEncryptionKey(msg, 0);
        }
  }


  incoming__before__Decrypt(client, msg);
}

void
incoming(int client, int msg) {
    if (__SELECTED_FEATURE_Decrypt) {
        incoming__role__Decrypt(client, msg);
    } else {
        incoming__before__Decrypt(client, msg);
    }
}




int createClient(char *name) {
    int client = initClient();
    return client;
}


void
sendEmail (int sender, int receiver)
{
  int email = createEmail (0, receiver);
  outgoing (sender, email);


}


void
queue (int client, int msg)
{
    queue_empty = 0;
    queued_message = msg;
    queued_client = client;
}


int
is_queue_empty ()
{
    return queue_empty;
}


int
get_queued_client ()
{
    return queued_client;
}


int
get_queued_email ()
{
    return queued_message;
}

int
isKeyPairValid (int publicKey, int privateKey)
{

  if (!publicKey || !privateKey)
    return 0;
  return privateKey == publicKey;
}


void
generateKeyPair (int client, int seed)
{
    setClientPrivateKey(client, seed);
}

void
autoRespond (int client, int msg)
{

  int sender = getEmailFrom(msg);
  setEmailTo(msg, sender);
  queue(client, msg);
}

void
sendToAddressBook (int client, int msg)
{

}

void
sign (int client, int msg)
{
  int privkey = getClientPrivateKey(client);
  if (!privkey)
    return;
  setEmailIsSigned(msg, 1);
  setEmailSignKey(msg, privkey);
}

void
forward (int client, int msg)
{

  printMail(msg);
  queue(client, msg);

}

void
verify (int client, int msg)
{

  if (!isReadable (msg) || !isSigned(msg))
    return;

  int pubkey = findPublicKey(client, getEmailFrom(msg));
  if (pubkey && isKeyPairValid(getEmailSignKey(msg), pubkey)) {
    setEmailIsSignatureVerified(msg, 1);
  }

}
# 1 "ClientLib.c"
# 1 "<eingebaut>"
# 1 "<Kommandozeile>"
# 1 "ClientLib.c"

# 1 "ClientLib.h" 1



int initClient();

char* getClientName(int handle);

void setClientName(int handle, char* value);

int getClientOutbuffer(int handle);

void setClientOutbuffer(int handle, int value);

int getClientAddressBookSize(int handle);

void setClientAddressBookSize(int handle, int value);

int createClientAddressBookEntry(int handle);

int getClientAddressBookAlias(int handle, int index);

void setClientAddressBookAlias(int handle, int index, int value);

int getClientAddressBookAddress(int handle, int index);

void setClientAddressBookAddress(int handle, int index, int value);


int getClientAutoResponse(int handle);

void setClientAutoResponse(int handle, int value);

int getClientPrivateKey(int handle);

void setClientPrivateKey(int handle, int value);

int getClientKeyringSize(int handle);

int createClientKeyringEntry(int handle);

int getClientKeyringUser(int handle, int index);

void setClientKeyringUser(int handle, int index, int value);

int getClientKeyringPublicKey(int handle, int index);

void setClientKeyringPublicKey(int handle, int index, int value);

int getClientForwardReceiver(int handle);

void setClientForwardReceiver(int handle, int value);

int getClientId(int handle);

void setClientId(int handle, int value);

int findPublicKey(int handle, int userid);

int findClientAddressBookAlias(int handle, int userid);
# 3 "ClientLib.c" 2

int __ste_Client_counter = 0;

int initClient() {
 if (__ste_Client_counter < 3) {
  return ++__ste_Client_counter;
 } else {
  return -1;
 }
}

char* __ste_client_name0 = 0;

char* __ste_client_name1 = 0;

char* __ste_client_name2 = 0;


char* getClientName(int handle) {
 if (handle == 1) {
  return __ste_client_name0;
 } else if (handle == 2) {
  return __ste_client_name1;
 } else if (handle == 3) {
  return __ste_client_name2;
 } else {
  return 0;
 }
}

void setClientName(int handle, char* value) {
 if (handle == 1) {
  __ste_client_name0 = value;
 } else if (handle == 2) {
  __ste_client_name1 = value;
 } else if (handle == 3) {
  __ste_client_name2 = value;
 }
}

int __ste_client_outbuffer0 = 0;

int __ste_client_outbuffer1 = 0;

int __ste_client_outbuffer2 = 0;

int __ste_client_outbuffer3 = 0;


int getClientOutbuffer(int handle) {
 if (handle == 1) {
  return __ste_client_outbuffer0;
 } else if (handle == 2) {
  return __ste_client_outbuffer1;
 } else if (handle == 3) {
  return __ste_client_outbuffer2;
 } else {
  return 0;
 }
}

void setClientOutbuffer(int handle, int value) {
 if (handle == 1) {
  __ste_client_outbuffer0 = value;
 } else if (handle == 2) {
  __ste_client_outbuffer1 = value;
 } else if (handle == 3) {
  __ste_client_outbuffer2 = value;
 }
}



int __ste_ClientAddressBook_size0 = 0;

int __ste_ClientAddressBook_size1 = 0;

int __ste_ClientAddressBook_size2 = 0;


int getClientAddressBookSize(int handle){
 if (handle == 1) {
  return __ste_ClientAddressBook_size0;
 } else if (handle == 2) {
  return __ste_ClientAddressBook_size1;
 } else if (handle == 3) {
  return __ste_ClientAddressBook_size2;
 } else {
  return 0;
 }
}

void setClientAddressBookSize(int handle, int value) {
 if (handle == 1) {
  __ste_ClientAddressBook_size0 = value;
 } else if (handle == 2) {
  __ste_ClientAddressBook_size1 = value;
 } else if (handle == 3) {
  __ste_ClientAddressBook_size2 = value;
 }
}

int createClientAddressBookEntry(int handle){
    int size = getClientAddressBookSize(handle);
 if (size < 3) {
     setClientAddressBookSize(handle, size + 1);
  return size + 1;
 } else return -1;
}


int __ste_Client_AddressBook0_Alias0 = 0;

int __ste_Client_AddressBook0_Alias1 = 0;

int __ste_Client_AddressBook0_Alias2 = 0;

int __ste_Client_AddressBook1_Alias0 = 0;

int __ste_Client_AddressBook1_Alias1 = 0;

int __ste_Client_AddressBook1_Alias2 = 0;

int __ste_Client_AddressBook2_Alias0 = 0;

int __ste_Client_AddressBook2_Alias1 = 0;

int __ste_Client_AddressBook2_Alias2 = 0;


int getClientAddressBookAlias(int handle, int index) {
 if (handle == 1) {
  if (index == 0) {
   return __ste_Client_AddressBook0_Alias0;
  } else if (index == 1) {
   return __ste_Client_AddressBook0_Alias1;
  } else if (index == 2) {
   return __ste_Client_AddressBook0_Alias2;
  } else {
   return 0;
  }
 } else if (handle == 2) {
  if (index == 0) {
   return __ste_Client_AddressBook1_Alias0;
  } else if (index == 1) {
   return __ste_Client_AddressBook1_Alias1;
  } else if (index == 2) {
   return __ste_Client_AddressBook1_Alias2;
  } else {
   return 0;
  }
 } else if (handle == 3) {
  if (index == 0) {
   return __ste_Client_AddressBook2_Alias0;
  } else if (index == 1) {
   return __ste_Client_AddressBook2_Alias1;
  } else if (index == 2) {
   return __ste_Client_AddressBook2_Alias2;
  } else {
   return 0;
  }
 } else {
  return 0;
 }
}


int findClientAddressBookAlias(int handle, int userid) {
 if (handle == 1) {
  if (userid == __ste_Client_AddressBook0_Alias0) {
   return 0;
  } else if (userid == __ste_Client_AddressBook0_Alias1) {
   return 1;
  } else if (userid == __ste_Client_AddressBook0_Alias2) {
   return 2;
  } else {
   return -1;
  }
 } else if (handle == 2) {
  if (userid == __ste_Client_AddressBook1_Alias0) {
   return 0;
  } else if (userid == __ste_Client_AddressBook1_Alias1) {
   return 1;
  } else if (userid == __ste_Client_AddressBook1_Alias2) {
   return 2;
  } else {
   return -1;
  }
 } else if (handle == 3) {
  if (userid == __ste_Client_AddressBook2_Alias0) {
   return 0;
  } else if (userid == __ste_Client_AddressBook2_Alias1) {
   return 1;
  } else if (userid == __ste_Client_AddressBook2_Alias2) {
   return 2;
  } else {
   return -1;
  }
 } else {
  return -1;
 }
}


void setClientAddressBookAlias(int handle, int index, int value) {
 if (handle == 1) {
  if (index == 0) {
   __ste_Client_AddressBook0_Alias0 = value;
  } else if (index == 1) {
   __ste_Client_AddressBook0_Alias1 = value;
  } else if (index == 2) {
   __ste_Client_AddressBook0_Alias2 = value;
  }
 } else if (handle == 2) {
  if (index == 0) {
   __ste_Client_AddressBook1_Alias0 = value;
  } else if (index == 1) {
   __ste_Client_AddressBook1_Alias1 = value;
  } else if (index == 2) {
   __ste_Client_AddressBook1_Alias2 = value;
  }
 } else if (handle == 3) {
  if (index == 0) {
   __ste_Client_AddressBook2_Alias0 = value;
  } else if (index == 1) {
   __ste_Client_AddressBook2_Alias1 = value;
  } else if (index == 2) {
   __ste_Client_AddressBook2_Alias2 = value;
  }
 }
}

int __ste_Client_AddressBook0_Address0 = 0;

int __ste_Client_AddressBook0_Address1 = 0;

int __ste_Client_AddressBook0_Address2 = 0;

int __ste_Client_AddressBook1_Address0 = 0;

int __ste_Client_AddressBook1_Address1 = 0;

int __ste_Client_AddressBook1_Address2 = 0;

int __ste_Client_AddressBook2_Address0 = 0;

int __ste_Client_AddressBook2_Address1 = 0;

int __ste_Client_AddressBook2_Address2 = 0;


int getClientAddressBookAddress(int handle, int index) {
 if (handle == 1) {
  if (index == 0) {
   return __ste_Client_AddressBook0_Address0;
  } else if (index == 1) {
   return __ste_Client_AddressBook0_Address1;
  } else if (index == 2) {
   return __ste_Client_AddressBook0_Address2;
  } else {
   return 0;
  }
 } else if (handle == 2) {
  if (index == 0) {
   return __ste_Client_AddressBook1_Address0;
  } else if (index == 1) {
   return __ste_Client_AddressBook1_Address1;
  } else if (index == 2) {
   return __ste_Client_AddressBook1_Address2;
  } else {
   return 0;
  }
 } else if (handle == 3) {
  if (index == 0) {
   return __ste_Client_AddressBook2_Address0;
  } else if (index == 1) {
   return __ste_Client_AddressBook2_Address1;
  } else if (index == 2) {
   return __ste_Client_AddressBook2_Address2;
  } else {
   return 0;
  }
 } else {
  return 0;
 }
}

void setClientAddressBookAddress(int handle, int index, int value) {
 if (handle == 1) {
  if (index == 0) {
   __ste_Client_AddressBook0_Address0 = value;
  } else if (index == 1) {
   __ste_Client_AddressBook0_Address1 = value;
  } else if (index == 2) {
   __ste_Client_AddressBook0_Address2 = value;
  }
 } else if (handle == 2) {
  if (index == 0) {
   __ste_Client_AddressBook1_Address0 = value;
  } else if (index == 1) {
   __ste_Client_AddressBook1_Address1 = value;
  } else if (index == 2) {
   __ste_Client_AddressBook1_Address2 = value;
  }
 } else if (handle == 3) {
  if (index == 0) {
   __ste_Client_AddressBook2_Address0 = value;
  } else if (index == 1) {
   __ste_Client_AddressBook2_Address1 = value;
  } else if (index == 2) {
   __ste_Client_AddressBook2_Address2 = value;
  }
 }
}

int __ste_client_autoResponse0 = 0;

int __ste_client_autoResponse1 = 0;

int __ste_client_autoResponse2 = 0;


int getClientAutoResponse(int handle) {
 if (handle == 1) {
  return __ste_client_autoResponse0;
 } else if (handle == 2) {
  return __ste_client_autoResponse1;
 } else if (handle == 3) {
  return __ste_client_autoResponse2;
 } else {
  return -1;
 }
}

void setClientAutoResponse(int handle, int value) {
 if (handle == 1) {
  __ste_client_autoResponse0 = value;
 } else if (handle == 2) {
  __ste_client_autoResponse1 = value;
 } else if (handle == 3) {
  __ste_client_autoResponse2 = value;
 }
}

int __ste_client_privateKey0 = 0;

int __ste_client_privateKey1 = 0;

int __ste_client_privateKey2 = 0;


int getClientPrivateKey(int handle) {
 if (handle == 1) {
  return __ste_client_privateKey0;
 } else if (handle == 2) {
  return __ste_client_privateKey1;
 } else if (handle == 3) {
  return __ste_client_privateKey2;
 } else {
  return 0;
 }
}

void setClientPrivateKey(int handle, int value) {
 if (handle == 1) {
  __ste_client_privateKey0 = value;
 } else if (handle == 2) {
  __ste_client_privateKey1 = value;
 } else if (handle == 3) {
  __ste_client_privateKey2 = value;
 }
}

int __ste_ClientKeyring_size0 = 0;

int __ste_ClientKeyring_size1 = 0;

int __ste_ClientKeyring_size2 = 0;


int getClientKeyringSize(int handle){
 if (handle == 1) {
  return __ste_ClientKeyring_size0;
 } else if (handle == 2) {
  return __ste_ClientKeyring_size1;
 } else if (handle == 3) {
  return __ste_ClientKeyring_size2;
 } else {
  return 0;
 }
}

void setClientKeyringSize(int handle, int value) {
 if (handle == 1) {
  __ste_ClientKeyring_size0 = value;
 } else if (handle == 2) {
  __ste_ClientKeyring_size1 = value;
 } else if (handle == 3) {
  __ste_ClientKeyring_size2 = value;
 }
}

int createClientKeyringEntry(int handle){
    int size = getClientKeyringSize(handle);
 if (size < 2) {
     setClientKeyringSize(handle, size + 1);
  return size + 1;
 } else return -1;
}

int __ste_Client_Keyring0_User0 = 0;

int __ste_Client_Keyring0_User1 = 0;

int __ste_Client_Keyring0_User2 = 0;

int __ste_Client_Keyring1_User0 = 0;

int __ste_Client_Keyring1_User1 = 0;

int __ste_Client_Keyring1_User2 = 0;

int __ste_Client_Keyring2_User0 = 0;

int __ste_Client_Keyring2_User1 = 0;

int __ste_Client_Keyring2_User2 = 0;


int getClientKeyringUser(int handle, int index) {
 if (handle == 1) {
  if (index == 0) {
   return __ste_Client_Keyring0_User0;
  } else if (index == 1) {
   return __ste_Client_Keyring0_User1;
  }

      else {
   return 0;
  }
 } else if (handle == 2) {
  if (index == 0) {
   return __ste_Client_Keyring1_User0;
  } else if (index == 1) {
   return __ste_Client_Keyring1_User1;
  }

      else {
   return 0;
  }
 } else if (handle == 3) {
  if (index == 0) {
   return __ste_Client_Keyring2_User0;
  } else if (index == 1) {
   return __ste_Client_Keyring2_User1;
  }

      else {
   return 0;
  }
 } else {
  return 0;
 }
}





void setClientKeyringUser(int handle, int index, int value) {
 if (handle == 1) {
  if (index == 0) {
   __ste_Client_Keyring0_User0 = value;
  } else if (index == 1) {
   __ste_Client_Keyring0_User1 = value;
  }


 } else if (handle == 2) {
  if (index == 0) {
   __ste_Client_Keyring1_User0 = value;
  } else if (index == 1) {
   __ste_Client_Keyring1_User1 = value;
  }


 } else if (handle == 3) {
  if (index == 0) {
   __ste_Client_Keyring2_User0 = value;
  } else if (index == 1) {
   __ste_Client_Keyring2_User1 = value;
  }


 }
}

int __ste_Client_Keyring0_PublicKey0 = 0;

int __ste_Client_Keyring0_PublicKey1 = 0;

int __ste_Client_Keyring0_PublicKey2 = 0;

int __ste_Client_Keyring1_PublicKey0 = 0;

int __ste_Client_Keyring1_PublicKey1 = 0;

int __ste_Client_Keyring1_PublicKey2 = 0;

int __ste_Client_Keyring2_PublicKey0 = 0;

int __ste_Client_Keyring2_PublicKey1 = 0;

int __ste_Client_Keyring2_PublicKey2 = 0;


int getClientKeyringPublicKey(int handle, int index) {
 if (handle == 1) {
  if (index == 0) {
   return __ste_Client_Keyring0_PublicKey0;
  } else if (index == 1) {
   return __ste_Client_Keyring0_PublicKey1;
  }

      else {
   return 0;
  }
 } else if (handle == 2) {
  if (index == 0) {
   return __ste_Client_Keyring1_PublicKey0;
  } else if (index == 1) {
   return __ste_Client_Keyring1_PublicKey1;
  }

      else {
   return 0;
  }
 } else if (handle == 3) {
  if (index == 0) {
   return __ste_Client_Keyring2_PublicKey0;
  } else if (index == 1) {
   return __ste_Client_Keyring2_PublicKey1;
  }

      else {
   return 0;
  }
 } else {
  return 0;
 }
}


int findPublicKey(int handle, int userid) {

 if (handle == 1) {
  if (userid == __ste_Client_Keyring0_User0) {
   return __ste_Client_Keyring0_PublicKey0;
  } else if (userid == __ste_Client_Keyring0_User1) {
   return __ste_Client_Keyring0_PublicKey1;
  }

      else {
   return 0;
  }
 } else if (handle == 2) {
  if (userid == __ste_Client_Keyring1_User0) {
   return __ste_Client_Keyring1_PublicKey0;
  } else if (userid == __ste_Client_Keyring1_User1) {
   return __ste_Client_Keyring1_PublicKey1;
  }

      else {
   return 0;
  }
 } else if (handle == 3) {
  if (userid == __ste_Client_Keyring2_User0) {
   return __ste_Client_Keyring2_PublicKey0;
  } else if (userid == __ste_Client_Keyring2_User1) {
   return __ste_Client_Keyring2_PublicKey1;
  }

      else {
   return 0;
  }
 } else {
  return 0;
 }
}


void setClientKeyringPublicKey(int handle, int index, int value) {
 if (handle == 1) {
  if (index == 0) {
   __ste_Client_Keyring0_PublicKey0 = value;
  } else if (index == 1) {
   __ste_Client_Keyring0_PublicKey1 = value;
  }


 } else if (handle == 2) {
  if (index == 0) {
   __ste_Client_Keyring1_PublicKey0 = value;
  } else if (index == 1) {
   __ste_Client_Keyring1_PublicKey1 = value;
  }


 } else if (handle == 3) {
  if (index == 0) {
   __ste_Client_Keyring2_PublicKey0 = value;
  } else if (index == 1) {
   __ste_Client_Keyring2_PublicKey1 = value;
  }


 }
}

int __ste_client_forwardReceiver0 =0;

int __ste_client_forwardReceiver1 =0;

int __ste_client_forwardReceiver2 =0;

int __ste_client_forwardReceiver3 =0;

int getClientForwardReceiver(int handle) {
 if (handle == 1) {
  return __ste_client_forwardReceiver0;
 } else if (handle == 2) {
  return __ste_client_forwardReceiver1;
 } else if (handle == 3) {
  return __ste_client_forwardReceiver2;
 } else {
  return 0;
 }
}

void setClientForwardReceiver(int handle, int value) {
 if (handle == 1) {
  __ste_client_forwardReceiver0 = value;
 } else if (handle == 2) {
  __ste_client_forwardReceiver1 = value;
 } else if (handle == 3) {
  __ste_client_forwardReceiver2 = value;
 }
}

int __ste_client_idCounter0 = 0;

int __ste_client_idCounter1 = 0;

int __ste_client_idCounter2 = 0;


int getClientId(int handle) {
 if (handle == 1) {
  return __ste_client_idCounter0;
 } else if (handle == 2) {
  return __ste_client_idCounter1;
 } else if (handle == 3) {
  return __ste_client_idCounter2;
 } else {
  return 0;
 }
}

void setClientId(int handle, int value) {
 if (handle == 1) {
  __ste_client_idCounter0 = value;
 } else if (handle == 2) {
  __ste_client_idCounter1 = value;
 } else if (handle == 3) {
  __ste_client_idCounter2 = value;
 }
}
# 1 "Email.c"
# 1 "<eingebaut>"
# 1 "<Kommandozeile>"
# 1 "Email.c"

# 1 "Email.h" 1
# 1 "featureselect.h" 1






int __SELECTED_FEATURE_Base;

int __SELECTED_FEATURE_Keys;

int __SELECTED_FEATURE_Encrypt;

int __SELECTED_FEATURE_AutoResponder;

int __SELECTED_FEATURE_AddressBook;

int __SELECTED_FEATURE_Sign;

int __SELECTED_FEATURE_Forward;

int __SELECTED_FEATURE_Verify;

int __SELECTED_FEATURE_Decrypt;

int __GUIDSL_ROOT_PRODUCTION;


int select_one();
void select_features();
void select_helpers();
int valid_product();
# 2 "Email.h" 2

# 1 "EmailLib.h" 1



int initEmail();

int getEmailId(int handle);

void setEmailId(int handle, int value);

int getEmailFrom(int handle);

void setEmailFrom(int handle, int value);

int getEmailTo(int handle);

void setEmailTo(int handle, int value);

char* getEmailSubject(int handle);

void setEmailSubject(int handle, char* value);

char* getEmailBody(int handle);

void setEmailBody(int handle, char* value);

int isEncrypted(int handle);

void setEmailIsEncrypted(int handle, int value);

int getEmailEncryptionKey(int handle);

void setEmailEncryptionKey(int handle, int value);

int isSigned(int handle);

void setEmailIsSigned(int handle, int value);

int getEmailSignKey(int handle);

void setEmailSignKey(int handle, int value);

int isVerified(int handle);

void setEmailIsSignatureVerified(int handle, int value);
# 4 "Email.h" 2


void printMail (int msg);


int isReadable (int msg);


int createEmail (int from, int to);


int cloneEmail(int msg);
# 3 "Email.c" 2



void
printMail__before__Encrypt (int msg)
{






}



void
printMail__role__Encrypt (int msg)
{
  printMail__before__Encrypt(msg);


}
void
printMail__before__Sign(int msg) {
    if (__SELECTED_FEATURE_Encrypt) {
        printMail__role__Encrypt(msg);
    } else {
        printMail__before__Encrypt(msg);
    }
}


void
printMail__role__Sign (int msg)
{
  printMail__before__Sign(msg);


}


void
printMail__before__Verify(int msg) {
    if (__SELECTED_FEATURE_Sign) {
        printMail__role__Sign(msg);
    } else {
        printMail__before__Sign(msg);
    }
}




void
printMail__role__Verify (int msg)
{
  printMail__before__Verify(msg);

}
void
printMail(int msg) {
    if (__SELECTED_FEATURE_Verify) {
        printMail__role__Verify(msg);
    } else {
        printMail__before__Verify(msg);
    }
}




int
isReadable__before__Encrypt (int msg)
{
  return 1;
}


int
isReadable__role__Encrypt (int msg)
{
  if (!isEncrypted(msg))
    return isReadable__before__Encrypt(msg);
  else
    return 0;
}



int
isReadable(int msg) {
    if (__SELECTED_FEATURE_Encrypt) {
        return isReadable__role__Encrypt(msg);
    } else {
        return isReadable__before__Encrypt(msg);
    }
}




int cloneEmail(int msg) {
    return msg;
}


int createEmail (int from, int to) {
  int msg = 1;
  setEmailFrom(msg, from);
  setEmailTo(msg, to);
  return msg;
}
# 1 "EmailLib.c"
# 1 "<eingebaut>"
# 1 "<Kommandozeile>"
# 1 "EmailLib.c"

# 1 "EmailLib.h" 1



int initEmail();

int getEmailId(int handle);

void setEmailId(int handle, int value);

int getEmailFrom(int handle);

void setEmailFrom(int handle, int value);

int getEmailTo(int handle);

void setEmailTo(int handle, int value);

char* getEmailSubject(int handle);

void setEmailSubject(int handle, char* value);

char* getEmailBody(int handle);

void setEmailBody(int handle, char* value);

int isEncrypted(int handle);

void setEmailIsEncrypted(int handle, int value);

int getEmailEncryptionKey(int handle);

void setEmailEncryptionKey(int handle, int value);

int isSigned(int handle);

void setEmailIsSigned(int handle, int value);

int getEmailSignKey(int handle);

void setEmailSignKey(int handle, int value);

int isVerified(int handle);

void setEmailIsSignatureVerified(int handle, int value);
# 3 "EmailLib.c" 2

int __ste_Email_counter = 0;

int initEmail() {
 if (__ste_Email_counter < 2) {
  return ++__ste_Email_counter;
 } else {
  return -1;
 }
}

int __ste_email_id0 = 0;

int __ste_email_id1 = 0;

int getEmailId(int handle) {
 if (handle == 1) {
  return __ste_email_id0;
 } else if (handle == 2) {
  return __ste_email_id1;
 } else {
  return 0;
 }
}

void setEmailId(int handle, int value) {
 if (handle == 1) {
  __ste_email_id0 = value;
 } else if (handle == 2) {
  __ste_email_id1 = value;
 }
}

int __ste_email_from0 = 0;

int __ste_email_from1 = 0;

int getEmailFrom(int handle) {
 if (handle == 1) {
  return __ste_email_from0;
 } else if (handle == 2) {
  return __ste_email_from1;
 } else {
  return 0;
 }
}

void setEmailFrom(int handle, int value) {
 if (handle == 1) {
  __ste_email_from0 = value;
 } else if (handle == 2) {
  __ste_email_from1 = value;
 }
}

int __ste_email_to0 = 0;

int __ste_email_to1 = 0;

int getEmailTo(int handle) {
 if (handle == 1) {
  return __ste_email_to0;
 } else if (handle == 2) {
  return __ste_email_to1;
 } else {
  return 0;
 }
}

void setEmailTo(int handle, int value) {
 if (handle == 1) {
  __ste_email_to0 = value;
 } else if (handle == 2) {
  __ste_email_to1 = value;
 }
}

char* __ste_email_subject0;

char* __ste_email_subject1;

char* getEmailSubject(int handle) {
 if (handle == 1) {
  return __ste_email_subject0;
 } else if (handle == 2) {
  return __ste_email_subject1;
 } else {
  return 0;
 }
}

void setEmailSubject(int handle, char* value) {
 if (handle == 1) {
  __ste_email_subject0 = value;
 } else if (handle == 2) {
  __ste_email_subject1 = value;
 }
}

char* __ste_email_body0 = 0;

char* __ste_email_body1 = 0;

char* getEmailBody(int handle) {
 if (handle == 1) {
  return __ste_email_body0;
 } else if (handle == 2) {
  return __ste_email_body1;
 } else {
  return 0;
 }
}

void setEmailBody(int handle, char* value) {
 if (handle == 1) {
  __ste_email_body0 = value;
 } else if (handle == 2) {
  __ste_email_body1 = value;
 }
}

int __ste_email_isEncrypted0 = 0;

int __ste_email_isEncrypted1 = 0;

int isEncrypted(int handle) {
 if (handle == 1) {
  return __ste_email_isEncrypted0;
 } else if (handle == 2) {
  return __ste_email_isEncrypted1;
 } else {
  return 0;
 }
}

void setEmailIsEncrypted(int handle, int value) {
 if (handle == 1) {
  __ste_email_isEncrypted0 = value;
 } else if (handle == 2) {
  __ste_email_isEncrypted1 = value;
 }
}

int __ste_email_encryptionKey0 = 0;

int __ste_email_encryptionKey1 = 0;

int getEmailEncryptionKey(int handle) {
 if (handle == 1) {
  return __ste_email_encryptionKey0;
 } else if (handle == 2) {
  return __ste_email_encryptionKey1;
 } else {
  return 0;
 }
}

void setEmailEncryptionKey(int handle, int value) {
 if (handle == 1) {
  __ste_email_encryptionKey0 = value;
 } else if (handle == 2) {
  __ste_email_encryptionKey1 = value;
 }
}

int __ste_email_isSigned0 = 0;

int __ste_email_isSigned1 = 0;

int isSigned(int handle) {
 if (handle == 1) {
  return __ste_email_isSigned0;
 } else if (handle == 2) {
  return __ste_email_isSigned1;
 } else {
  return 0;
 }
}

void setEmailIsSigned(int handle, int value) {
 if (handle == 1) {
  __ste_email_isSigned0 = value;
 } else if (handle == 2) {
  __ste_email_isSigned1 = value;
 }
}

int __ste_email_signKey0 = 0;

int __ste_email_signKey1 = 0;

int getEmailSignKey(int handle) {
 if (handle == 1) {
  return __ste_email_signKey0;
 } else if (handle == 2) {
  return __ste_email_signKey1;
 } else {
  return 0;
 }
}

void setEmailSignKey(int handle, int value) {
 if (handle == 1) {
  __ste_email_signKey0 = value;
 } else if (handle == 2) {
  __ste_email_signKey1 = value;
 }
}

int __ste_email_isSignatureVerified0;

int __ste_email_isSignatureVerified1;

int isVerified(int handle) {
 if (handle == 1) {
  return __ste_email_isSignatureVerified0;
 } else if (handle == 2) {
  return __ste_email_isSignatureVerified1;
 } else {
  return 0;
 }
}

void setEmailIsSignatureVerified(int handle, int value) {
 if (handle == 1) {
  __ste_email_isSignatureVerified0 = value;
 } else if (handle == 2) {
  __ste_email_isSignatureVerified1 = value;
 }
}
# 1 "featureselect.c"
# 1 "<eingebaut>"
# 1 "<Kommandozeile>"
# 1 "featureselect.c"
# 1 "featureselect.h" 1






int __SELECTED_FEATURE_Base;

int __SELECTED_FEATURE_Keys;

int __SELECTED_FEATURE_Encrypt;

int __SELECTED_FEATURE_AutoResponder;

int __SELECTED_FEATURE_AddressBook;

int __SELECTED_FEATURE_Sign;

int __SELECTED_FEATURE_Forward;

int __SELECTED_FEATURE_Verify;

int __SELECTED_FEATURE_Decrypt;

int __GUIDSL_ROOT_PRODUCTION;


int select_one();
void select_features();
void select_helpers();
int valid_product();
# 2 "featureselect.c" 2






extern int __VERIFIER_nondet_int(void);
int select_one() {if (__VERIFIER_nondet_int()) return 1; else return 0;}


void select_features() {
 __SELECTED_FEATURE_Base = 1;
 __SELECTED_FEATURE_Keys = select_one();
 __SELECTED_FEATURE_Encrypt = select_one();
 __SELECTED_FEATURE_AutoResponder = select_one();
 __SELECTED_FEATURE_AddressBook = select_one();
 __SELECTED_FEATURE_Sign = select_one();
 __SELECTED_FEATURE_Forward = select_one();
 __SELECTED_FEATURE_Verify = select_one();
 __SELECTED_FEATURE_Decrypt = select_one();
}


void select_helpers() {
 __GUIDSL_ROOT_PRODUCTION = 1;
}

int valid_product() {
  return (( !__SELECTED_FEATURE_Encrypt || __SELECTED_FEATURE_Decrypt ) && ( !__SELECTED_FEATURE_Decrypt || __SELECTED_FEATURE_Encrypt )) && ( !__SELECTED_FEATURE_Encrypt || __SELECTED_FEATURE_Keys ) && (( !__SELECTED_FEATURE_Sign || __SELECTED_FEATURE_Verify ) && ( !__SELECTED_FEATURE_Verify || __SELECTED_FEATURE_Sign )) && ( !__SELECTED_FEATURE_Sign || __SELECTED_FEATURE_Keys ) && ( __SELECTED_FEATURE_Base ) ;
}
# 1 "scenario.c"
# 1 "<eingebaut>"
# 1 "<Kommandozeile>"
# 1 "scenario.c"
# 1 "featureselect.h" 1






int __SELECTED_FEATURE_Base;

int __SELECTED_FEATURE_Keys;

int __SELECTED_FEATURE_Encrypt;

int __SELECTED_FEATURE_AutoResponder;

int __SELECTED_FEATURE_AddressBook;

int __SELECTED_FEATURE_Sign;

int __SELECTED_FEATURE_Forward;

int __SELECTED_FEATURE_Verify;

int __SELECTED_FEATURE_Decrypt;

int __GUIDSL_ROOT_PRODUCTION;


int select_one();
void select_features();
void select_helpers();
int valid_product();
# 2 "scenario.c" 2

void test() {
    int op1 = 0;
    int op2 = 0;
    int op3 = 0;
    int op4 = 0;
    int op5 = 0;
    int op6 = 0;
    int op7 = 0;
    int op8 = 0;
    int op9 = 0;
    int op10 = 0;
    int op11 = 0;
    int splverifierCounter = 0;
    while(splverifierCounter < 4) {
        op1 = 0;
        op2 = 0;
        op3 = 0;
        op4 = 0;
        op5 = 0;
        op6 = 0;
        op7 = 0;
        op8 = 0;
        op9 = 0;
        op10 = 0;
        op11 = 0;
        splverifierCounter = splverifierCounter + 1;
        if (!op1 && get_nondet()) {
            if (__SELECTED_FEATURE_Keys)
                bobKeyAdd();
            op1 = 1;
        }
        else if (!op2 && get_nondet()) {
            if (__SELECTED_FEATURE_AutoResponder)
                rjhSetAutoRespond();
            op2 = 1;
        }
        else if (!op3 && get_nondet()) {
            if (__SELECTED_FEATURE_Keys)
                rjhDeletePrivateKey();
            op3 = 1;
        }
        else if (!op4 && get_nondet()) {
            if (__SELECTED_FEATURE_Keys)
                rjhKeyAdd();
            op4 = 1;
        }
        else if (!op5 && get_nondet()) {
            if (__SELECTED_FEATURE_Keys)
                chuckKeyAddRjh();
            op5 = 1;
        }
        else if (!op6 && get_nondet()) {
            if (__SELECTED_FEATURE_Forward)
                rjhEnableForwarding();
            op6 = 1;
        }
        else if (!op7 && get_nondet()) {
            if (__SELECTED_FEATURE_Keys)
                rjhKeyChange();
            op7 = 1;
        }
        else if (!op8 && get_nondet()) {
            if (__SELECTED_FEATURE_AddressBook)
                bobSetAddressBook();
            op8 = 1;
        }
        else if (!op9 && get_nondet()) {
            if (__SELECTED_FEATURE_Keys)
                chuckKeyAdd();
            op9 = 1;
        }
        else if (!op10 && get_nondet()) {
            if (__SELECTED_FEATURE_Keys)
                bobKeyChange();
            op10 = 1;
        }
        else if (!op11 && get_nondet()) {
            if (__SELECTED_FEATURE_Keys)
                chuckKeyAdd();
            op11 = 1;
        }
        else break;
    }
        bobToRjh();
}
# 1 "Test.c"
# 1 "<eingebaut>"
# 1 "<Kommandozeile>"
# 1 "Test.c"



# 1 "Client.h" 1
# 1 "ClientLib.h" 1



int initClient();

char* getClientName(int handle);

void setClientName(int handle, char* value);

int getClientOutbuffer(int handle);

void setClientOutbuffer(int handle, int value);

int getClientAddressBookSize(int handle);

void setClientAddressBookSize(int handle, int value);

int createClientAddressBookEntry(int handle);

int getClientAddressBookAlias(int handle, int index);

void setClientAddressBookAlias(int handle, int index, int value);

int getClientAddressBookAddress(int handle, int index);

void setClientAddressBookAddress(int handle, int index, int value);


int getClientAutoResponse(int handle);

void setClientAutoResponse(int handle, int value);

int getClientPrivateKey(int handle);

void setClientPrivateKey(int handle, int value);

int getClientKeyringSize(int handle);

int createClientKeyringEntry(int handle);

int getClientKeyringUser(int handle, int index);

void setClientKeyringUser(int handle, int index, int value);

int getClientKeyringPublicKey(int handle, int index);

void setClientKeyringPublicKey(int handle, int index, int value);

int getClientForwardReceiver(int handle);

void setClientForwardReceiver(int handle, int value);

int getClientId(int handle);

void setClientId(int handle, int value);

int findPublicKey(int handle, int userid);

int findClientAddressBookAlias(int handle, int userid);
# 2 "Client.h" 2

# 1 "Email.h" 1
# 1 "featureselect.h" 1






int __SELECTED_FEATURE_Base;

int __SELECTED_FEATURE_Keys;

int __SELECTED_FEATURE_Encrypt;

int __SELECTED_FEATURE_AutoResponder;

int __SELECTED_FEATURE_AddressBook;

int __SELECTED_FEATURE_Sign;

int __SELECTED_FEATURE_Forward;

int __SELECTED_FEATURE_Verify;

int __SELECTED_FEATURE_Decrypt;

int __GUIDSL_ROOT_PRODUCTION;


int select_one();
void select_features();
void select_helpers();
int valid_product();
# 2 "Email.h" 2

# 1 "EmailLib.h" 1



int initEmail();

int getEmailId(int handle);

void setEmailId(int handle, int value);

int getEmailFrom(int handle);

void setEmailFrom(int handle, int value);

int getEmailTo(int handle);

void setEmailTo(int handle, int value);

char* getEmailSubject(int handle);

void setEmailSubject(int handle, char* value);

char* getEmailBody(int handle);

void setEmailBody(int handle, char* value);

int isEncrypted(int handle);

void setEmailIsEncrypted(int handle, int value);

int getEmailEncryptionKey(int handle);

void setEmailEncryptionKey(int handle, int value);

int isSigned(int handle);

void setEmailIsSigned(int handle, int value);

int getEmailSignKey(int handle);

void setEmailSignKey(int handle, int value);

int isVerified(int handle);

void setEmailIsSignatureVerified(int handle, int value);
# 4 "Email.h" 2


void printMail (int msg);


int isReadable (int msg);


int createEmail (int from, int to);


int cloneEmail(int msg);
# 4 "Client.h" 2

# 1 "EmailLib.h" 1



int initEmail();

int getEmailId(int handle);

void setEmailId(int handle, int value);

int getEmailFrom(int handle);

void setEmailFrom(int handle, int value);

int getEmailTo(int handle);

void setEmailTo(int handle, int value);

char* getEmailSubject(int handle);

void setEmailSubject(int handle, char* value);

char* getEmailBody(int handle);

void setEmailBody(int handle, char* value);

int isEncrypted(int handle);

void setEmailIsEncrypted(int handle, int value);

int getEmailEncryptionKey(int handle);

void setEmailEncryptionKey(int handle, int value);

int isSigned(int handle);

void setEmailIsSigned(int handle, int value);

int getEmailSignKey(int handle);

void setEmailSignKey(int handle, int value);

int isVerified(int handle);

void setEmailIsSignatureVerified(int handle, int value);
# 6 "Client.h" 2
# 14 "Client.h"
void queue (int client, int msg);


int is_queue_empty ();

int get_queued_client ();

int get_queued_email ();


void mail (int client, int msg);

void outgoing (int client, int msg);

void deliver (int client, int msg);

void incoming (int client, int msg);

int createClient(char *name);


void sendEmail (int sender, int receiver);



int
isKeyPairValid (int publicKey, int privateKey);


void
generateKeyPair (int client, int seed);
void autoRespond (int client, int msg);
void sendToAddressBook (int client, int msg);

void sign (int client, int msg);

void forward (int client, int msg);

void verify (int client, int msg);
# 5 "Test.c" 2

# 1 "Test.h" 1

int bob;


int rjh;


int chuck;


void setup_bob(int bob);


void setup_rjh(int rjh);


void setup_chuck(int chuck);


void before();


void bobToRjh();


void rjhToBob();


void test();


void setup();


int main();
void bobKeyAdd();


void bobKeyAddChuck();


void rjhKeyAdd();


void rjhKeyAddChuck();


void chuckKeyAdd();


void bobKeyChange();


void rjhKeyChange();


void rjhDeletePrivateKey();


void chuckKeyAddRjh();
void rjhSetAutoRespond();

void bobSetAddressBook();
void rjhEnableForwarding();
# 7 "Test.c" 2

# 1 "Util.h" 1
int prompt(char* msg);
# 9 "Test.c" 2




void
setup_bob__before__Keys(int bob)
{
    setClientId(bob, bob);
}




void
setup_bob__role__Keys(int bob)
{
  setup_bob__before__Keys(bob);
  setClientPrivateKey(bob, 123);

}
void
setup_bob(int bob) {
    if (__SELECTED_FEATURE_Keys) {
        setup_bob__role__Keys(bob);
    } else {
        setup_bob__before__Keys(bob);
    }
}





void
setup_rjh__before__Keys(int rjh)
{
    setClientId(rjh, rjh);
}



void
setup_rjh__role__Keys(int rjh)
{

  setup_rjh__before__Keys(rjh);
  setClientPrivateKey(rjh, 456);

}


void
setup_rjh(int rjh) {
    if (__SELECTED_FEATURE_Keys) {
        setup_rjh__role__Keys(rjh);
    } else {
        setup_rjh__before__Keys(rjh);
    }
}




void
setup_chuck__before__Keys(int chuck)
{
    setClientId(chuck, chuck);
}


void
setup_chuck__role__Keys(int chuck)
{
  setup_chuck__before__Keys(chuck);
  setClientPrivateKey(chuck, 789);
}


void
setup_chuck(int chuck) {
    if (__SELECTED_FEATURE_Keys) {
        setup_chuck__role__Keys(chuck);
    } else {
        setup_chuck__before__Keys(chuck);
    }
}
# 103 "Test.c"
void
bobToRjh()
{


  sendEmail(bob,rjh);
  if (!is_queue_empty()) {
    outgoing(get_queued_client(), get_queued_email());
  }
}


void
rjhToBob()
{


  sendEmail(rjh,bob);
}


int get_nondet() {
    int nd;
    return nd;
}



void setup() {
  bob = 1;
  setup_bob(bob);


  rjh = 2;
  setup_rjh(rjh);


  chuck = 3;
  setup_chuck(chuck);

}


int
main (void)
{
  select_helpers();
  select_features();
  if (valid_product()) {
      setup();
      test();
  }

}


void
bobKeyAdd()
{
    createClientKeyringEntry(bob);
    setClientKeyringUser(bob, 0, 2);
    setClientKeyringPublicKey(bob, 0, 456);



}


void
rjhKeyAdd()
{
    createClientKeyringEntry(rjh);
    setClientKeyringUser(rjh, 0, 1);
    setClientKeyringPublicKey(rjh, 0, 123);
}


void
rjhKeyAddChuck()
{
    createClientKeyringEntry(rjh);
    setClientKeyringUser(rjh, 0, 3);
    setClientKeyringPublicKey(rjh, 0, 789);
}



void
bobKeyAddChuck()
{
    createClientKeyringEntry(bob);
    setClientKeyringUser(bob, 1, 3);
    setClientKeyringPublicKey(bob, 1, 789);
}


void
chuckKeyAdd()
{
    createClientKeyringEntry(chuck);
    setClientKeyringUser(chuck, 0, 1);
    setClientKeyringPublicKey(chuck, 0, 123);
}


void
chuckKeyAddRjh()
{
    createClientKeyringEntry(chuck);
    setClientKeyringUser(chuck, 0, 2);
    setClientKeyringPublicKey(chuck, 0, 456);
}


void
rjhDeletePrivateKey()
{
    setClientPrivateKey(rjh, 0);
}


void
bobKeyChange()
{
  generateKeyPair(bob, 777);
}


void
rjhKeyChange()
{
  generateKeyPair(rjh, 666);
}


void
rjhSetAutoRespond()
{
  setClientAutoResponse(rjh, 1);
}

void
bobSetAddressBook()
{
    setClientAddressBookSize(bob, 1);
    setClientAddressBookAlias(bob, 0, rjh);
    setClientAddressBookAddress(bob, 0, rjh);
    setClientAddressBookAddress(bob, 1, chuck);
}


void
rjhEnableForwarding()
{
  setClientForwardReceiver(rjh, chuck);
}
# 1 "Util.c"
# 1 "<eingebaut>"
# 1 "<Kommandozeile>"
# 1 "Util.c"

# 1 "Util.h" 1
int prompt(char* msg);
# 3 "Util.c" 2



int
prompt(char* msg)
{

    int retval;

    return retval;
}
