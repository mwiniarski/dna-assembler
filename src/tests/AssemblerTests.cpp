#define private public
#include "../Assembler.h"
#undef private
#include <sstream>

namespace std
{
  ostream& operator<<(ostream& s, const pair<int,int>& p) {
    s << '<' << p.first << ',' << p.second << '>';
    return s;
  }
}


#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE(AssemblerTests)

BOOST_AUTO_TEST_CASE(assembler_load_reads)
{
    Assembler a("src/tests/reads", 16, 0);
    a.loadReads();
    BOOST_CHECK_EQUAL(a.reads[0], "AACGCTAGCTAGCTAG");
    BOOST_CHECK_EQUAL(a.reads[2], "CCCGGTTACGGATTGC");
}

BOOST_AUTO_TEST_CASE(assembler_compare_reads)
{
    Assembler a("src/tests/reads", 16, 0);
    a.loadReads();
    int cr = a.compareReads(a.reads[1],a.reads[2]);
    BOOST_CHECK_EQUAL(cr, 7);
}

BOOST_AUTO_TEST_CASE(assembler_compare_reads_reverse)
{
    Assembler a("src/tests/reads", 16, 0);
    a.loadReads();
    int cr = a.compareReads(a.reads[1],a.reads[0]);
    BOOST_CHECK_EQUAL(cr, -4);
}

BOOST_AUTO_TEST_CASE(assembler_compare_reads_no_match)
{
    Assembler a("src/tests/reads", 16, 0);
    a.loadReads();
    int cr = a.compareReads(a.reads[0],a.reads[2]);
    BOOST_CHECK_EQUAL(cr, 0);
}

BOOST_AUTO_TEST_CASE(assembler_join_reads)
{
    Assembler a("src/tests/reads", 16, 0);
    a.loadReads();
    int cr = a.compareReads(a.reads[1],a.reads[2]);
    std::string str = a.joinReads(a.reads[1],a.reads[2], cr);
    BOOST_CHECK_EQUAL(str, "CTAGATCGTCCCGGTTACGGATTGC");
}

BOOST_AUTO_TEST_CASE(assembler_compare)
{
    Assembler a("src/tests/reads", 16, 0);
    a.loadReads();
    a.compare(1,2);
    BOOST_CHECK_EQUAL(a.table[1].front().first, 2);
    BOOST_CHECK_EQUAL(a.table[1].front().second, 7);
    BOOST_CHECK_EQUAL(a.table[2].empty(), true);
}

BOOST_AUTO_TEST_CASE(assembler_compare_all)
{
    Assembler a("src/tests/reads", 16, 0);
    a.loadReads();
    a.compareAll();
    auto pair = std::pair<unsigned, unsigned>(1,4);
    BOOST_CHECK_EQUAL(a.table[0].front(), pair);
    pair = std::pair<unsigned, unsigned>(2,7);
    BOOST_CHECK_EQUAL(a.table[1].front(), pair);

    auto i = a.table[3].begin();
    pair = std::pair<unsigned, unsigned>(0,12);
    BOOST_CHECK_EQUAL(*i++, pair);

    pair = std::pair<unsigned, unsigned>(1,4);
    BOOST_CHECK_EQUAL(*i, pair);

    BOOST_CHECK_EQUAL(a.table[0].size(), 1);
    BOOST_CHECK_EQUAL(a.table[1].size(), 1);
    BOOST_CHECK_EQUAL(a.table[2].size(), 0);
    BOOST_CHECK_EQUAL(a.table[3].size(), 2);
}

BOOST_AUTO_TEST_CASE(assembler_join)
{
    Assembler a("src/tests/reads", 16, 0);
    a.loadReads();
    a.compareAll();
    a.join();
    BOOST_CHECK_EQUAL(a.reads[0], "");
    BOOST_CHECK_EQUAL(a.reads[1], "CTAGATCGTCCCGGTT");
    BOOST_CHECK_EQUAL(a.reads[2], "CCCGGTTACGGATTGC");
    BOOST_CHECK_EQUAL(a.reads[3], "ACTCAACGCTAGCTAGCTAG");
}

BOOST_AUTO_TEST_CASE(assembler_run)
{
    Assembler a("src/tests/reads", 16, 4);
    a.run();
    BOOST_CHECK_EQUAL(a.reads[3], "ACTCAACGCTAGCTAGCTAGATCGTCCCGGTTACGGATTGC");
}

BOOST_AUTO_TEST_SUITE_END()
