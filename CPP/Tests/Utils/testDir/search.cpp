#include "tester.h"
#include "search.h"

using namespace std;

int noSuchMatch(testLogger& log);
int SearchNoContext(testLogger& log);
int SearchNoResults(testLogger& log);
int Next(testLogger& log);
int Prev(testLogger& log);
int IsMatch(testLogger& log);

vector<string> input = {
    "The quick brown fox jumped over the lazy dog",
    "Is a string that is used all the the time",
    "Because it has every letter of the english alphabet in it",
    "I assume there are more that are used, but this is the only one I have heard off"
};

int main(int argc, const char *argv[])
{
    Test("Checking search returns correct return value...",noSuchMatch).RunTest();
    Test("Checking searchers return NoPos() when no search...",SearchNoContext).RunTest();
    Test("Checking searchers return NoPos() when there are no results...",SearchNoResults).RunTest();
    Test("Checking Next returns correct values when there are results...",Next).RunTest();
    Test("Checking Prev returns correct values when there are results...",Prev).RunTest();
    Test("Checking IsMatch...",IsMatch).RunTest();
    return 0;
}

int noSuchMatch(testLogger& log ) {
    Searcher searcher(input);
    if ( searcher.Search("This doesn't exist!") ) {
        log << "Searcher matched a non existent string!" << endl;
        return 1;
    }
    if ( searcher.Search("fo(.*") ) {
        log << "Searcher matched an invalid regex" << endl;
        return 1;
    }
    if ( !searcher.Search("fox") ) {
        log << "Searcher failed to match a sub string" << endl;
        return 1;
    }
    if ( !searcher.Search("The quick brown fox jumped over the lazy dog") ) {
        log << "Searcher failed to match a whole line" << endl;
        return 1;
    }
    if ( !searcher.Search("f[oa]x") ) {
        log << "Searcher failed to match a simple regex" << endl;
        return 1;
    }
    return 0;
}

int SearchNoContext(testLogger& log) {
    Searcher searcher(input);
    if ( searcher.Next(input.begin()) != searcher.NoPos() ) {
        log << "searcher: Next did not return NoPos! " << endl;
        return 1;
    }
    if ( searcher.Prev(input.begin()) != searcher.NoPos() ) {
        log << "searcher: Prev did not return NoPos! " << endl;
        return 1;
    }
    if ( searcher.First() != searcher.NoPos() ) {
        log << "searcher: First did not return NoPos! " << endl;
        return 1;
    }
    if ( searcher.Last() != searcher.NoPos() ) {
        log << "searcher: Last did not return NoPos! " << endl;
        return 1;
    }
    std::vector<string> emptyInput;

    Searcher empty(emptyInput);
    if ( empty.Next(emptyInput.begin()) != empty.NoPos() ) {
        log << "empty: Next did not return NoPos! " << endl;
        return 1;
    }
    if ( empty.Prev(emptyInput.begin()) != empty.NoPos() ) {
        log << "empty: Prev did not return NoPos! " << endl;
        return 1;
    }

    if ( empty.First() != empty.NoPos() ) {
        log << "empty: First did not return NoPos! " << endl;
        return 1;
    }
    if ( empty.Last() != empty.NoPos() ) {
        log << "empty: Last did not return NoPos! " << endl;
        return 1;
    }
    return 0;
}

int SearchNoResults(testLogger& log) {
    Searcher searcher(input);
    searcher.Search("XXXXXXXXXXXXXX");
    if ( searcher.Next(input.begin()) != searcher.NoPos() ) {
        log << "searcher: Next did not return NoPos! " << endl;
        return 1;
    }
    if ( searcher.Prev(input.begin()) != searcher.NoPos() ) {
        log << "searcher: Prev did not return NoPos! " << endl;
        return 1;
    }
    if ( searcher.First() != searcher.NoPos() ) {
        log << "searcher: First did not return NoPos! " << endl;
        return 1;
    }
    if ( searcher.Last() != searcher.NoPos() ) {
        log << "searcher: Last did not return NoPos! " << endl;
        return 1;
    }
    std::vector<string> emptyInput;

    Searcher empty(emptyInput);
    empty.Search("XXXXXXXXXXXXXXXXXXXXX");
    if ( empty.Next(emptyInput.begin()) != empty.NoPos() ) {
        log << "empty: Next did not return NoPos! " << endl;
        return 1;
    }
    if ( empty.Prev(emptyInput.begin()) != empty.NoPos() ) {
        log << "empty: Prev did not return NoPos! " << endl;
        return 1;
    }
    if ( empty.First() != empty.NoPos() ) {
        log << "empty: First did not return NoPos! " << endl;
        return 1;
    }
    if ( empty.Last() != empty.NoPos() ) {
        log << "empty: Last did not return NoPos! " << endl;
        return 1;
    }
    return 0;
}

int Next(testLogger& log ) {
    Searcher searcher(input);
    /*
     * Check we can find the first line!
     */
    searcher.Search("the");
    auto theit = searcher.First();
    if ( theit == searcher.NoPos() || *theit != input[0] ) {
        log << " Next failed to find the first line!" << endl;
        return 1;
    }

    /*
     * Two matches (line 1 [string] and line 2 [in])
     */
    searcher.Search("in");
    auto it = searcher.First();
    if ( it == searcher.NoPos() || *it != input[1] ) {
        log << " Next failed to return the first result!" << endl;
        return 1;
    }
    it = searcher.Next(it);
    if ( it == searcher.NoPos() || *it != input[2] ) {
        log << " Next failed to return the second result!" << endl;
        return 1;
    }
    it = searcher.Next(it);

    if ( it != searcher.NoPos()) {
        log << " Next returned a third result!" << endl;
        return 1;
    }

    it = searcher.Next(it);
    if ( it != searcher.NoPos()) {
        log << " Next returned a post end result!" << endl;
        return 1;
    }
    return 0;
}

int Prev(testLogger& log ) {
    Searcher searcher(input);
    searcher.Search("the");
    /*
     * Check we can find the last line!
     */
    auto moreIt = searcher.Last();
    if( moreIt == searcher.NoPos() || *moreIt != input[3] ) {
        log << "Searcher failed to find the last element!" << endl;
        return 1;
    }
    /*
     * Two matches (line 1 [string] and line 2 [in])
     */
    searcher.Search("in");
    auto it = searcher.Last();
    if ( it == searcher.NoPos() || *it != input[2] ) {
        log << " Prev failed to return the first result!" << endl;
        return 1;
    }
    it = searcher.Prev(it);
    if ( it == searcher.NoPos() || *it != input[1] ) {
        log << " Prev failed to return the second result!" << endl;
        return 1;
    }
    it = searcher.Prev(it);

    if ( it != searcher.NoPos()) {
        log << " Prev returned a third result!" << endl;
        return 1;
    }

    return 0;
}

int IsMatch(testLogger& log ) {
    Searcher searcher(input);
    if ( searcher.IsMatch(input.begin())) {
        log << "IsMatch returned true when no search!" << endl;
        return 1;
    }
    searcher.Search("fox");
    if ( !searcher.IsMatch(input.begin())) {
        log << "IsMatch failed to find the first line" << endl;
        return 1;
    }
    if ( searcher.IsMatch(input.begin()+1)) {
        log << "IsMatch found a result on the second line!" << endl;
        return 1;
    }
    searcher.Reset();
    if ( searcher.IsMatch(input.begin())) {
        log << "IsMatch matched a result after a reset!" << endl;
        return 1;
    }
    return 0;
}
