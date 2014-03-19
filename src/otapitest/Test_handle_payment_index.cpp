#include "stdafx.hpp"


TEST_MULTI(handle_payment_index)
{
	EXPECT_MULTI_RETURN("", NYM_ID, GetAccountWallet_NymID(ACCOUNT_ID));

	EXPECT_MULTI_RETURN("", SERVER_ID, GetAccountWallet_ServerID(ACCOUNT_ID));

	OtMeTest::EXPECT_get_payment_instrument(mock, index, SERVER_ID, NYM_ID, 0, INBOX_DATA);

	EXPECT_MULTI_RETURN("", PAYMENT_TYPE_CHEQUE, Instrmnt_GetType(INSTRUMENT_DATA));

	EXPECT_MULTI_RETURN(NYM_TO_ID, NYM_ID, Instrmnt_GetRecipientUserID(INSTRUMENT_DATA));

	if (index >= 0)
	{
		//FIX what if this one fails?
		EXPECT_CALL_RETURN(ASSET_ID, Instrmnt_GetAssetID(INSTRUMENT_DATA));

		//FIX what if this one fails?
		// fails when ASSET_ID does not match ASSET_ID
		EXPECT_MULTI_RETURN(ASSET_NAME, ASSET_ID, GetAccountWallet_AssetTypeID(ACCOUNT_ID));

		if (index >= 0)
		{
			//FIX what if OT_ERROR?
			EXPECT_MULTI_RETURN(mock.GetTime() + 864000000L, mock.GetTime() - 864000000L, Instrmnt_GetValidFrom(INSTRUMENT_DATA)); // 10 days ago

			//FIX what if OT_ERROR?
			//FIX failure causes RecordPayment() ? OT_FALSE : OT_ERROR
			EXPECT_CALL_RETURN(mock.GetTime() + 864000000L, Instrmnt_GetValidTo(INSTRUMENT_DATA)); // 10 days ahead

			//FIX different for PAYMENT_TYPE_PURSE
			OtMeTest::EXPECT_details_deposit_cheque(mock, index, SERVER_ID, ACCOUNT_ID, NYM_ID, INSTRUMENT_DATA, PAYMENT_TYPE_CHEQUE);
		}
	}

	ASSERT_MULTI_EQ(OT_ERROR, OT_TRUE, me.handle_payment_index(ACCOUNT_ID, 0, PAYMENT_TYPE_CHEQUE, INBOX_DATA));
}


int OtMeChai::handle_payment_index(const char * accountId, int32_t paymentIndex, const char * paymentType, const char * inboxData)
{
    OTString code;
    code.Format("{ handle_payment_index(\"%s\", %d, \"%s\", \"%s\"); }",
                accountId, paymentIndex, paymentType, inboxData);
    return execInt(code.Get());
}

int OtMeExtra::handle_payment_index(const char * accountId, int32_t paymentIndex, const char * paymentType, const char * inboxData)
{
    return ::handle_payment_index(accountId, paymentIndex, paymentType, inboxData);
}


void OtMeTest::EXPECT_handle_payment_index(Mock_OTAPI_Exec & mock, int & index, const char * accountId, int paymentIndex, const char * paymentType, const char * inboxData)
{
	EXPECT_CALL_RETURN(NYM_ID, GetAccountWallet_NymID(accountId));

	EXPECT_CALL_RETURN(SERVER_ID, GetAccountWallet_ServerID(accountId));

	OtMeTest::EXPECT_get_payment_instrument(mock, noAltPathways, SERVER_ID, NYM_ID, paymentIndex, inboxData);

	//FIX definitely tinker with these
	EXPECT_CALL_RETURN(paymentType, Instrmnt_GetType(INSTRUMENT_DATA));

	EXPECT_CALL_RETURN(NYM_ID, Instrmnt_GetRecipientUserID(INSTRUMENT_DATA));

	EXPECT_CALL_RETURN(ASSET_ID, Instrmnt_GetAssetID(INSTRUMENT_DATA));

	EXPECT_CALL_RETURN(ASSET_ID, GetAccountWallet_AssetTypeID(accountId));

	//FIX definitely tinker with these
	EXPECT_CALL_RETURN(mock.GetTime() - 864000000L, Instrmnt_GetValidFrom(INSTRUMENT_DATA)); // 10 days ago

	EXPECT_CALL_RETURN(mock.GetTime() + 864000000L, Instrmnt_GetValidTo(INSTRUMENT_DATA)); // 10 days ahead

	OtMeTest::EXPECT_details_deposit_cheque(mock, index, SERVER_ID, accountId, NYM_ID, INSTRUMENT_DATA, PAYMENT_TYPE_CHEQUE);
}