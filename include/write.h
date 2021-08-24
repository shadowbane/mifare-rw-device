/**
 * Send welcome message to user
 */
void sendWelcomeMessage() {
    Serial.println("> Welcome to YV ID Card Writer <");
    Serial.println("Please put card next to the writer");
}

/**
 * Read the card using set key
 */
bool readIdentifier(MFRC522::MIFARE_Key key)
{
    //some variables we need
    byte block = 1;
    byte len = 18;
    byte buffer1[18];
    MFRC522::StatusCode status;

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Authentication failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        Serial.println(F("\n**End Reading**\n"));

        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        return false;
    }

    status = mfrc522.MIFARE_Read(block, buffer1, &len);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("Reading failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        Serial.println(F("\n**End Reading**\n"));

        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        return false;
    }

    // buzzer
    tone(buzzer, 5000); // Send 1KHz sound signal...
    digitalWrite(led, HIGH);
    delay(100);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    digitalWrite(led, LOW);

    delay(50);        // ...for 1 sec

    tone(buzzer, 5000); // Send 1KHz sound signal...
    digitalWrite(led, HIGH);
    delay(100);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    digitalWrite(led, LOW);

    //PRINT Identifier
    char var;
    String Identifier;
    for (uint8_t i = 0; i < 16; i++)
    {
        if (buffer1[i] != 32)
        {
            var = buffer1[i];
            if (var != '\n' && var != '\r') {
                Identifier.concat(String(var));
            }
        }
    }

    Serial.print(F("Identifier: "));
    Serial.print(Identifier);
    Serial.println("");
    Serial.println("");

    return true;
}

void execWriter()
{
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    Serial.println(F("**Card Detected:**"));

    //dump some details about the card
    mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid));

    if (!readIdentifier(key)) {
        return;
    }

    byte buffer[34];
    byte block;
    byte len;
    MFRC522::StatusCode status;

    // Ask personal data: Identifier
    Serial.setTimeout(10000L); // wait until 20 seconds for input from serial
    Serial.println(F("Type Identifier, ending with #"));
    len = Serial.readBytesUntil('#', (char *)buffer, 20); // read Identifier from serial
    for (byte i = len; i < 20; i++) {
        buffer[i] = ' '; // pad with spaces
    }

    block = 1;

    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Write block 1
    status = mfrc522.MIFARE_Write(block, buffer, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    } else {
        Serial.print(F("MIFARE_Write() success! \n"));
    }

    Serial.println(F("\n**End Writing**\n"));
    delay(1000); //change value if you want to read cards faster
    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    sendWelcomeMessage();
}