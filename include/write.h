void execWriter()
{
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
    MFRC522::MIFARE_Key key;
    // for (byte i = 0; i < 6; i++)
        key.keyByte[0] = 0xB7;
        key.keyByte[1] = 0x17;
        key.keyByte[2] = 0xEB;
        key.keyByte[3] = 0xA1;
        key.keyByte[4] = 0x23;
        key.keyByte[5] = 0x45;

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    Serial.print(F("Card UID:")); //Dump UID
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.print(F(" PICC type: ")); // Dump PICC type
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    byte buffer[34];
    byte block;
    MFRC522::StatusCode status;
    byte len;

    Serial.setTimeout(20000L); // wait until 20 seconds for input from serial
    // Ask personal data: NIM
    Serial.println(F("Type NIM, ending with #"));
    len = Serial.readBytesUntil('#', (char *)buffer, 20); // read NIM from serial
    for (byte i = len; i < 20; i++)
        buffer[i] = ' '; // pad with spaces

    block = 1;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Write block
    status = mfrc522.MIFARE_Write(block, buffer, 16);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else
        Serial.println(F("MIFARE_Write() success: "));

    block = 2;
    //Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Write block
    status = mfrc522.MIFARE_Write(block, &buffer[16], 16);
    if (status != MFRC522::STATUS_OK)
    {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    else
        Serial.println(F("MIFARE_Write() success: "));

    Serial.println(F("\n**End Reading**\n"));
    delay(1000); //change value if you want to read cards faster

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    readerMode = "";
    Serial.println("type help for info");
    my_cli();
}