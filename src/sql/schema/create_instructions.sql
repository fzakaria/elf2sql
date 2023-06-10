CREATE TABLE IF NOT EXISTS Instructions
(
    id       INTEGER PRIMARY KEY,
    mnemonic TEXT,
    operands TEXT,
    raw      BLOB -- the raw value of the instruction
);