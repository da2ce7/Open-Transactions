#include "stdafx.hpp"


TEST_MULTI(ot_utility_ReceiveReplyLowLevel)
{
	EXPECT_CALL_VOID(Sleep(50));

	EXPECT_MULTI_RETURN("", MESSAGE_DATA, PopMessageBuffer(REQUEST_NUMBER, SERVER_ID, NYM_ID));

	ASSERT_MULTI_EQ("", MESSAGE_DATA, me.ot_utility_ReceiveReplyLowLevel(SERVER_ID, NYM_ID, REQUEST_NUMBER, __FUNCTION__));
}


string OtMeChai::ot_utility_ReceiveReplyLowLevel(const char * serverId, const char * nymId, int reqNr, const char * function)
{
	OTString code;
	code.Format("{ var util = Utility(); util.ReceiveReplyLowLevel(\"%s\", \"%s\", %d, \"%s\"); }",
                serverId, nymId, reqNr, function);
	return execStr(code.Get());
}


string OtMeExtra::ot_utility_ReceiveReplyLowLevel(const char * serverId, const char * nymId, int reqNr, const char * function)
{
    Utility util;
    return util.ReceiveReplyLowLevel(serverId, nymId, reqNr, function);
}


void OtMeTest::EXPECT_ot_utility_ReceiveReplyLowLevel(Mock_OTAPI_Exec & mock, int & index, const char * serverId, const char * nymId, int reqNr, const char * function)
{
	if (index >= 0)
	{
		EXPECT_CALL_VOID(Sleep(50));

		EXPECT_MULTI_RETURN("", MESSAGE_DATA, PopMessageBuffer(reqNr, serverId, nymId));
	}
}