#include <tester.h>
#include <enum_value.h>

int DefaultValue(testLogger& log);
int InitialValue(testLogger& log);
int Copy(testLogger& log);
int StringValue(testLogger& log);
int FromString(testLogger& log);
int UseAsConstEnum(testLogger& log);
int UseAsMutableEnum(testLogger& log);
int StatusFlag(testLogger& log);
int CompareRaw(testLogger& log);
int CompareEnumVal(testLogger& log);

int main( int argc, char**argv) {
    Test("Default value is set correctly",DefaultValue).RunTest();
    Test("Initial value is set correctly",InitialValue).RunTest();
    Test("Copying data works correctly",Copy).RunTest();
    Test("Conversion to string",StringValue).RunTest();
    Test("Conversion from string",FromString).RunTest();
    Test("Use as const enum",UseAsConstEnum).RunTest();
    Test("Use as mutable enum",UseAsMutableEnum).RunTest();
    Test("Status Flag",StatusFlag).RunTest();
    Test("Compare with raw enums",CompareRaw).RunTest();
    Test("Compare with EnumVals",CompareEnumVal).RunTest();
    return 0;
}

enum VALUES {
    VALUE_1,
    VALUE_2,
    ERROR_VALUE
};

typedef EnumValue<VALUES,ERROR_VALUE> EValue;


template<>
EValue::Map EValue::string_map = {
    {"Value 1", VALUE_1},
    {"Value 2", VALUE_2},
    {"", ERROR_VALUE}
};

int DefaultValue(testLogger& log) {
    EValue e;

    if (e.value != ERROR_VALUE) {
        log << "enum was not initialised correctl!" << endl;
        return 1;
    }
    return 0;
}

int InitialValue(testLogger& log) {
    EValue e(VALUE_1);

    if (e.value != VALUE_1) {
        log << "enum was not initialised correctl!" << endl;
        return 1;
    }
    return 0;
}

int Copy(testLogger& log) {
    EValue e1 = EValue(VALUE_1);

    if (e1.value != VALUE_1) {
        log << "enum was not initialised correctl!" << endl;
        return 1;
    }

    e1 = EValue(VALUE_2);

    if (e1.value != VALUE_2) {
        log << "(2) enum was not initialised correctl!" << endl;
        return 1;
    }

    e1 = VALUE_1;

    if (e1.value != VALUE_1) {
        log << "(3) enum was not initialised correctl!" << endl;
        return 1;
    }

    return 0;
}

int StringValue(testLogger& log) {
    EValue e1 = EValue(VALUE_1);

    if ( e1.StringValue() != "Value 1") {
        log << "Invalid string value!" << endl;
        return 1;
    }

    e1 = static_cast<VALUES>(1000);

    if ( e1.StringValue() != e1.NO_SUCH_VALUE) {
        log << "Invalid string value for stupid enum val" << endl;
        return 1;
    }

    return 0;
}

int FromString(testLogger& log) {
    EValue e1 ("Value 1");

    if ( e1.value != VALUE_1) {
        log << "Invalid string value!" << endl;
        return 1;
    }

    e1.SetValue("Value 2");

    if ( e1.value != VALUE_2) {
        log << "(2) Invalid string value!" << endl;
        return 1;
    }

    return 0;
}

int UseAsConstEnum(testLogger& log) {
    EValue e = VALUE_2;
    const EValue e2 = VALUE_2;
    auto f_ref = [] (const VALUES& v) -> bool {
        return v == VALUE_2;
    };

    auto f = [] (VALUES v) -> bool {
        return v == VALUE_2;
    };

    if ( !f(e) ) {
        log << "Could not extract enum value!" << endl;
        return 1;
    }

    if ( !f(e2) ) {
        log << "Could not extract enum value from const!" << endl;
        return 1;
    }

    if ( !f_ref(e) ) {
        log << "Could not extract enum value!" << endl;
        return 1;
    }

    if ( !f_ref(e2) ) {
        log << "Could not extract enum value from const!" << endl;
        return 1;
    }
    return 0;
}

int UseAsMutableEnum(testLogger& log ) {
    auto f = [] (VALUES& v) -> void {
        v = VALUE_2;
    };

    EValue e;
    f(e);

    if ( e.value != VALUE_2) {
        log << "Value was not updated correctly!" << endl;
        return 1;
    }
    return 0;
}

int StatusFlag(testLogger& log) {
    EValue e;

    if ( e.IsOk() ) {
        log << "Defaulted enum is oK!" << endl;
        return 1;
    }

    e = VALUE_1;

    if ( !e.IsOk() ) {
        log << "Enum is not ok with a valid value!" << endl;
        return 1;
    }

    e = ERROR_VALUE;

    if ( e.IsOk() ) {
        log << "Error enum is oK!" << endl;
        return 1;
    }

    e = static_cast<VALUES>(1000);;

    if ( e.IsOk() ) {
        log << "Stupid enum is oK!" << endl;
        return 1;
    }

    return 0;
}

int CompareRaw(testLogger& log) {
    EValue e = VALUE_2;

    if ( e == VALUE_1 || VALUE_1 == e) {
        log << "Enum equals another enum value!" << endl;
        return 1;
    }

    if ( !(e == VALUE_2) || !(VALUE_2 == e) ) {
        log << "Enum failed compare against itself" << endl;
        return 1;
    }

    if ( e != VALUE_2 || VALUE_2 != e) {
        log << "Enum does not equal itself!" << endl;
        return 1;
    }

    if ( !(e != VALUE_1) || !(VALUE_1 != e)) {
        log << "Enum failed not equal against another!" << endl;
        return 1;
    }

    return 0;

}

int CompareEnumVal(testLogger& log) {
    EValue e = VALUE_2;

    EValue v1 = VALUE_1;
    EValue v2 = VALUE_2;

    if ( e == v1 || v1 == e) {
        log << "Enum equals another enum value!" << endl;
        return 1;
    }

    if ( !(e == v2) || !(v2 == e) ) {
        log << "Enum failed compare against itself" << endl;
        return 1;
    }

    if ( e != v2 || v2 != e) {
        log << "Enum does not equal itself!" << endl;
        return 1;
    }

    if ( !(e != v1) || !(v1 != e)) {
        log << "Enum failed not equal against another!" << endl;
        return 1;
    }

    return 0;

}
