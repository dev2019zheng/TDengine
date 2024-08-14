/*
 * Copyright (c) 2019 TAOS Data, Inc. <jhtao@taosdata.com>
 *
 * This program is free software: you can use, redistribute, and/or modify
 * it under the terms of the GNU Affero General Public License, version 3
 * or later ("AGPL"), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TD_COMMON_TOKEN_H_
#define _TD_COMMON_TOKEN_H_

#define TK_OR                               1
#define TK_AND                              2
#define TK_UNION                            3
#define TK_ALL                              4
#define TK_MINUS                            5
#define TK_EXCEPT                           6
#define TK_INTERSECT                        7
#define TK_NK_BITAND                        8
#define TK_NK_BITOR                         9
#define TK_NK_LSHIFT                       10
#define TK_NK_RSHIFT                       11
#define TK_NK_PLUS                         12
#define TK_NK_MINUS                        13
#define TK_NK_STAR                         14
#define TK_NK_SLASH                        15
#define TK_NK_REM                          16
#define TK_NK_CONCAT                       17
#define TK_CREATE                          18
#define TK_ACCOUNT                         19
#define TK_NK_ID                           20
#define TK_PASS                            21
#define TK_NK_STRING                       22
#define TK_ALTER                           23
#define TK_PPS                             24
#define TK_TSERIES                         25
#define TK_STORAGE                         26
#define TK_STREAMS                         27
#define TK_QTIME                           28
#define TK_DBS                             29
#define TK_USERS                           30
#define TK_CONNS                           31
#define TK_STATE                           32
#define TK_NK_COMMA                        33
#define TK_HOST                            34
#define TK_IS_IMPORT                       35
#define TK_NK_INTEGER                      36
#define TK_CREATEDB                        37
#define TK_USER                            38
#define TK_ENABLE                          39
#define TK_SYSINFO                         40
#define TK_ADD                             41
#define TK_DROP                            42
#define TK_GRANT                           43
#define TK_ON                              44
#define TK_TO                              45
#define TK_REVOKE                          46
#define TK_FROM                            47
#define TK_SUBSCRIBE                       48
#define TK_READ                            49
#define TK_WRITE                           50
#define TK_NK_DOT                          51
#define TK_WITH                            52
#define TK_ENCRYPT_KEY                     53
#define TK_DNODE                           54
#define TK_PORT                            55
#define TK_DNODES                          56
#define TK_RESTORE                         57
#define TK_NK_IPTOKEN                      58
#define TK_FORCE                           59
#define TK_UNSAFE                          60
#define TK_CLUSTER                         61
#define TK_LOCAL                           62
#define TK_QNODE                           63
#define TK_BNODE                           64
#define TK_SNODE                           65
#define TK_MNODE                           66
#define TK_VNODE                           67
#define TK_DATABASE                        68
#define TK_USE                             69
#define TK_FLUSH                           70
#define TK_TRIM                            71
#define TK_S3MIGRATE                       72
#define TK_COMPACT                         73
#define TK_IF                              74
#define TK_NOT                             75
#define TK_EXISTS                          76
#define TK_BUFFER                          77
#define TK_CACHEMODEL                      78
#define TK_CACHESIZE                       79
#define TK_COMP                            80
#define TK_DURATION                        81
#define TK_NK_VARIABLE                     82
#define TK_MAXROWS                         83
#define TK_MINROWS                         84
#define TK_KEEP                            85
#define TK_PAGES                           86
#define TK_PAGESIZE                        87
#define TK_TSDB_PAGESIZE                   88
#define TK_PRECISION                       89
#define TK_REPLICA                         90
#define TK_VGROUPS                         91
#define TK_SINGLE_STABLE                   92
#define TK_RETENTIONS                      93
#define TK_SCHEMALESS                      94
#define TK_WAL_LEVEL                       95
#define TK_WAL_FSYNC_PERIOD                96
#define TK_WAL_RETENTION_PERIOD            97
#define TK_WAL_RETENTION_SIZE              98
#define TK_WAL_ROLL_PERIOD                 99
#define TK_WAL_SEGMENT_SIZE               100
#define TK_STT_TRIGGER                    101
#define TK_TABLE_PREFIX                   102
#define TK_TABLE_SUFFIX                   103
#define TK_S3_CHUNKSIZE                   104
#define TK_S3_KEEPLOCAL                   105
#define TK_S3_COMPACT                     106
#define TK_KEEP_TIME_OFFSET               107
#define TK_ENCRYPT_ALGORITHM              108
#define TK_NK_COLON                       109
#define TK_BWLIMIT                        110
#define TK_START                          111
#define TK_TIMESTAMP                      112
#define TK_END                            113
#define TK_TABLE                          114
#define TK_NK_LP                          115
#define TK_NK_RP                          116
#define TK_USING                          117
#define TK_FILE                           118
#define TK_STABLE                         119
#define TK_COLUMN                         120
#define TK_MODIFY                         121
#define TK_RENAME                         122
#define TK_TAG                            123
#define TK_SET                            124
#define TK_NK_EQ                          125
#define TK_TAGS                           126
#define TK_BOOL                           127
#define TK_TINYINT                        128
#define TK_SMALLINT                       129
#define TK_INT                            130
#define TK_INTEGER                        131
#define TK_BIGINT                         132
#define TK_FLOAT                          133
#define TK_DOUBLE                         134
#define TK_BINARY                         135
#define TK_NCHAR                          136
#define TK_UNSIGNED                       137
#define TK_JSON                           138
#define TK_VARCHAR                        139
#define TK_MEDIUMBLOB                     140
#define TK_BLOB                           141
#define TK_VARBINARY                      142
#define TK_GEOMETRY                       143
#define TK_DECIMAL                        144
#define TK_COMMENT                        145
#define TK_MAX_DELAY                      146
#define TK_WATERMARK                      147
#define TK_ROLLUP                         148
#define TK_TTL                            149
#define TK_SMA                            150
#define TK_DELETE_MARK                    151
#define TK_FIRST                          152
#define TK_LAST                           153
#define TK_SHOW                           154
#define TK_FULL                           155
#define TK_PRIVILEGES                     156
#define TK_DATABASES                      157
#define TK_TABLES                         158
#define TK_STABLES                        159
#define TK_MNODES                         160
#define TK_QNODES                         161
#define TK_ARBGROUPS                      162
#define TK_FUNCTIONS                      163
#define TK_INDEXES                        164
#define TK_ACCOUNTS                       165
#define TK_APPS                           166
#define TK_CONNECTIONS                    167
#define TK_LICENCES                       168
#define TK_GRANTS                         169
#define TK_LOGS                           170
#define TK_MACHINES                       171
#define TK_ENCRYPTIONS                    172
#define TK_QUERIES                        173
#define TK_SCORES                         174
#define TK_TOPICS                         175
#define TK_VARIABLES                      176
#define TK_BNODES                         177
#define TK_SNODES                         178
#define TK_TRANSACTIONS                   179
#define TK_DISTRIBUTED                    180
#define TK_CONSUMERS                      181
#define TK_SUBSCRIPTIONS                  182
#define TK_VNODES                         183
#define TK_ALIVE                          184
#define TK_VIEWS                          185
#define TK_VIEW                           186
#define TK_COMPACTS                       187
#define TK_NORMAL                         188
#define TK_CHILD                          189
#define TK_LIKE                           190
#define TK_TBNAME                         191
#define TK_QTAGS                          192
#define TK_AS                             193
#define TK_SYSTEM                         194
#define TK_TSMA                           195
#define TK_INTERVAL                       196
#define TK_RECURSIVE                      197
#define TK_TSMAS                          198
#define TK_FUNCTION                       199
#define TK_INDEX                          200
#define TK_COUNT                          201
#define TK_LAST_ROW                       202
#define TK_META                           203
#define TK_ONLY                           204
#define TK_TOPIC                          205
#define TK_CONSUMER                       206
#define TK_GROUP                          207
#define TK_DESC                           208
#define TK_DESCRIBE                       209
#define TK_RESET                          210
#define TK_QUERY                          211
#define TK_CACHE                          212
#define TK_EXPLAIN                        213
#define TK_ANALYZE                        214
#define TK_VERBOSE                        215
#define TK_NK_BOOL                        216
#define TK_RATIO                          217
#define TK_NK_FLOAT                       218
#define TK_OUTPUTTYPE                     219
#define TK_AGGREGATE                      220
#define TK_BUFSIZE                        221
#define TK_LANGUAGE                       222
#define TK_REPLACE                        223
#define TK_STREAM                         224
#define TK_INTO                           225
#define TK_PAUSE                          226
#define TK_RESUME                         227
#define TK_PRIMARY                        228
#define TK_KEY                            229
#define TK_TRIGGER                        230
#define TK_AT_ONCE                        231
#define TK_WINDOW_CLOSE                   232
#define TK_FORCE_WINDOW_CLOSE             233
#define TK_IGNORE                         234
#define TK_EXPIRED                        235
#define TK_FILL_HISTORY                   236
#define TK_UPDATE                         237
#define TK_SUBTABLE                       238
#define TK_UNTREATED                      239
#define TK_KILL                           240
#define TK_CONNECTION                     241
#define TK_TRANSACTION                    242
#define TK_BALANCE                        243
#define TK_VGROUP                         244
#define TK_LEADER                         245
#define TK_MERGE                          246
#define TK_REDISTRIBUTE                   247
#define TK_SPLIT                          248
#define TK_DELETE                         249
#define TK_INSERT                         250
#define TK_NK_BIN                         251
#define TK_NK_HEX                         252
#define TK_NULL                           253
#define TK_NK_QUESTION                    254
#define TK_NK_ALIAS                       255
#define TK_NK_ARROW                       256
#define TK_ROWTS                          257
#define TK_QSTART                         258
#define TK_QEND                           259
#define TK_QDURATION                      260
#define TK_WSTART                         261
#define TK_WEND                           262
#define TK_WDURATION                      263
#define TK_IROWTS                         264
#define TK_ISFILLED                       265
#define TK_CAST                           266
#define TK_POSITION                       267
#define TK_IN                             268
#define TK_FOR                            269
#define TK_NOW                            270
#define TK_TODAY                          271
#define TK_SUBSTR                         272
#define TK_SUBSTRING                      273
#define TK_BOTH                           274
#define TK_TRAILING                       275
#define TK_LEADING                        276
#define TK_TIMEZONE                       277
#define TK_CLIENT_VERSION                 278
#define TK_SERVER_VERSION                 279
#define TK_SERVER_STATUS                  280
#define TK_CURRENT_USER                   281
#define TK_PI                             282
#define TK_CASE                           283
#define TK_WHEN                           284
#define TK_THEN                           285
#define TK_ELSE                           286
#define TK_BETWEEN                        287
#define TK_IS                             288
#define TK_NK_LT                          289
#define TK_NK_GT                          290
#define TK_NK_LE                          291
#define TK_NK_GE                          292
#define TK_NK_NE                          293
#define TK_MATCH                          294
#define TK_NMATCH                         295
#define TK_CONTAINS                       296
#define TK_JOIN                           297
#define TK_INNER                          298
#define TK_LEFT                           299
#define TK_RIGHT                          300
#define TK_OUTER                          301
#define TK_SEMI                           302
#define TK_ANTI                           303
#define TK_ASOF                           304
#define TK_WINDOW                         305
#define TK_WINDOW_OFFSET                  306
#define TK_JLIMIT                         307
#define TK_SELECT                         308
#define TK_NK_HINT                        309
#define TK_DISTINCT                       310
#define TK_WHERE                          311
#define TK_PARTITION                      312
#define TK_BY                             313
#define TK_SESSION                        314
#define TK_STATE_WINDOW                   315
#define TK_EVENT_WINDOW                   316
#define TK_COUNT_WINDOW                   317
#define TK_SLIDING                        318
#define TK_FILL                           319
#define TK_VALUE                          320
#define TK_VALUE_F                        321
#define TK_NONE                           322
#define TK_PREV                           323
#define TK_NULL_F                         324
#define TK_LINEAR                         325
#define TK_NEXT                           326
#define TK_HAVING                         327
#define TK_RANGE                          328
#define TK_EVERY                          329
#define TK_ORDER                          330
#define TK_SLIMIT                         331
#define TK_SOFFSET                        332
#define TK_LIMIT                          333
#define TK_OFFSET                         334
#define TK_ASC                            335
#define TK_NULLS                          336
#define TK_ABORT                          337
#define TK_AFTER                          338
#define TK_ATTACH                         339
#define TK_BEFORE                         340
#define TK_BEGIN                          341
#define TK_BITAND                         342
#define TK_BITNOT                         343
#define TK_BITOR                          344
#define TK_BLOCKS                         345
#define TK_CHANGE                         346
#define TK_COMMA                          347
#define TK_CONCAT                         348
#define TK_CONFLICT                       349
#define TK_COPY                           350
#define TK_DEFERRED                       351
#define TK_DELIMITERS                     352
#define TK_DETACH                         353
#define TK_DIVIDE                         354
#define TK_DOT                            355
#define TK_EACH                           356
#define TK_FAIL                           357
#define TK_GLOB                           358
#define TK_ID                             359
#define TK_IMMEDIATE                      360
#define TK_IMPORT                         361
#define TK_INITIALLY                      362
#define TK_INSTEAD                        363
#define TK_ISNULL                         364
#define TK_MODULES                        365
#define TK_NK_BITNOT                      366
#define TK_NK_SEMI                        367
#define TK_NOTNULL                        368
#define TK_OF                             369
#define TK_PLUS                           370
#define TK_PRIVILEGE                      371
#define TK_RAISE                          372
#define TK_RESTRICT                       373
#define TK_ROW                            374
#define TK_STAR                           375
#define TK_STATEMENT                      376
#define TK_STRICT                         377
#define TK_STRING                         378
#define TK_TIMES                          379
#define TK_VALUES                         380
#define TK_VARIABLE                       381
#define TK_WAL                            382
#define TK_ENCODE                         383
#define TK_COMPRESS                       384
#define TK_LEVEL                          385



#define TK_NK_SPACE   600
#define TK_NK_COMMENT 601
#define TK_NK_ILLEGAL 602
// #define TK_NK_HEX           603  // hex number  0x123
#define TK_NK_OCT 604  // oct number
// #define TK_NK_BIN           605  // bin format data 0b111
#define TK_BATCH_SCAN        606
#define TK_NO_BATCH_SCAN     607
#define TK_SORT_FOR_GROUP    608
#define TK_PARTITION_FIRST   609
#define TK_PARA_TABLES_SORT  610
#define TK_SMALLDATA_TS_SORT 611
#define TK_HASH_JOIN         612
#define TK_SKIP_TSMA         613

#define TK_NK_NIL 65535

#endif /*_TD_COMMON_TOKEN_H_*/
