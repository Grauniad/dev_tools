QUnit.module("Logger.Formater");

QUnit.test( "Format Warning", function( assert ) {
    msg = DEV_TOOLS_Log_Format("WARNING","Format Warning","message");
    exp = /^\n\[WARNING\] [0-9:. ]+ - Format Warning: \nmessage\n\n$/
      assert.ok(exp.test(msg),msg);
});

QUnit.test( "Debug Message", function( assert ) {
    msg = DEV_TOOLS_Log_Format("DEBUG 1","Debug Message","message");
    exp = /^\n\[DEBUG 1\] [0-9:. ]+ - Debug Message: \nmessage\n\n$/
      assert.ok(exp.test(msg),msg);
});

QUnit.test( "Invalid level", function( assert ) {
    assert.throws(
        function () { DEV_TOOLS_Log_Format("NOT A LEVEL","Debug Message","message")},
        Error("Unknown level: NOT A LEVEL"),
        "Did not get the epected exception for an invalid level!"
    );
});

QUnit.test( "Missing message", function( assert ) {
    assert.throws(
        function () { DEV_TOOLS_Log_Format("WARNING","Debug Message")},
        Error("No value provided for argument: log_message"),
        "Did not get the epected exception for a missing log message!"
    );
});

QUnit.test( "Missing context", function( assert ) {
    assert.throws(
        function () { DEV_TOOLS_Log_Format("WARNING")},
        Error("No value provided for argument: log_context"),
        "Did not get the epected exception for a missing log context!"
    );
});

QUnit.test( "Missing level", function( assert ) {
    assert.throws(
        function () { DEV_TOOLS_Log_Format()},
        Error("No value provided for argument: log_level"),
        "Did not get the epected exception for a missing log level!"
    );
});