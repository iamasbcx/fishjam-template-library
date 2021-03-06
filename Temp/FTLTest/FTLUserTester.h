#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* 
************************************************************************/

class CFTLUserTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CFTLUserTester );
    CPPUNIT_TEST( dump_TokenInfo );
    CPPUNIT_TEST( dump_NamedObjectSecurityInformation );
    CPPUNIT_TEST( dump_WellKnownSid );

    CPPUNIT_TEST( test_IsVistaUACEnabled );

    CPPUNIT_TEST_SUITE_END();

    void dump_TokenInfo();
    void dump_NamedObjectSecurityInformation();
    void dump_WellKnownSid();

    void test_IsVistaUACEnabled();


    DECLARE_DEFAULT_TEST_CLASS(CFTLUserTester);
private:
    static unsigned int __stdcall _DumpThreadTokenProc(void* pParam);
};
