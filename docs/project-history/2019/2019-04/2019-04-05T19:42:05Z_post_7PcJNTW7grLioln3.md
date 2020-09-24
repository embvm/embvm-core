Binary size significantly reduced (60%) in release mode by removing demangling code. This code was being linked in, even though we aren't using RTTI or demangling calls.
