# (c) Copyright 2007-2012 Synapse Wireless, Inc.
"""NV Parameter ID Constants"""


NV_MAC_ADDR_ID = 2
NV_NETWORK_ID = 3
NV_CHANNEL_ID = 4

NV_GROUP_INTEREST_MASK_ID = 5   #which xCAST groups we will process
NV_GROUP_FORWARDING_MASK_ID = 6 #which xCAST groups we will forward (TTL permitting)

#"Date of Manufacturing"
NV_MFG_DATE_ID = 7

NV_DEVICE_NAME_ID = 8

NV_SYSTEM_ERROR_ID = 9

#"Personality"
NV_DEVICE_TYPE_ID = 10

#"Feature Bits"
NV_FEATURE_BITS_ID = 11 #see DeviceConfig.h for possible values

NV_DEFAULT_UART_ID = 12 #see Uart.h for possible values

#Some performance knobs for DATA MODE
NV_UART_DM_TIMEOUT_ID = 13 #how many milliseconds MAX until data pushed to other end
NV_UART_DM_THRESHOLD_ID = 14 #how big to let packet get before pushing to other end
NV_UART_DM_INTERCHAR_ID = 15 #like the original timeout, but this one is between characters

NV_CARRIER_SENSE_ID = 16 #listen before you speak (all)
NV_COLLISION_DETECT_ID = 17 #listen after you speak (multicast only)
NV_COLLISION_AVOIDANCE_ID = 18 # use random backoff (all)

NV_SNAP_MAX_RETRIES_ID = 19 # max retries for unicast

#Mesh Routing Related
NV_MESH_ROUTE_AGE_MAX_TIMEOUT_ID = 20
NV_MESH_ROUTE_AGE_MIN_TIMEOUT_ID = 21
NV_MESH_ROUTE_NEW_TIMEOUT_ID     = 22
NV_MESH_ROUTE_USED_TIMEOUT_ID    = 23
NV_MESH_ROUTE_DELETE_TIMEOUT_ID  = 24

NV_MESH_RREQ_TRIES_ID            = 25
NV_MESH_RREQ_WAIT_TIME_ID        = 26
NV_MESH_INITIAL_HOPLIMIT_ID      = 27
NV_MESH_MAX_HOPLIMIT_ID          = 28

NV_MESH_SEQUENCE_NUMBER_ID       = 29
NV_MESH_OVERRIDE_ID              = 30
NV_MESH_PENALTY_LQ_THRESHOLD_ID  = 31 # count weak RREP as an extra hop
NV_MESH_REJECTION_LQ_THRESHOLD_ID = 32 # disqualify weak RREP completely

NV_CS_CD_LEVEL_ID                = 33 # LQ (higher = weaker) at which we think another node is talking (ex. RF300)

NV_SNAP_LQ_THRESHOLD_ID          = 39

# Snappy Related
NV_SNAPPY_CRC_ID                 = 40
NV_SYS_PLATFORM_ID               = 41

#Encryption/Decryption/Security Related
NV_CRYPTO_TYPE = NV_AES128_ENABLE_ID = 50 #Boolean
NV_CRYPTO_KEY = NV_AES128_KEY_ID = 51 #16 character string
NV_LOCKDOWN_FLAGS_ID = 52 # 16 "bit-flags"
NV_MAX_LOYALTY_ID = 53 # Applies to Si1000 Only
# may someday support alternate crypto TYPE = 54
NV_ALT_CRYPTO_KEY = NV_ALTERNATE_KEY_ID = 55 #16 character string, just like NV #51

# "Copy Protection" (demo mode) related
NV_LAST_VERSION_BOOTED_ID = 60
NV_REBOOTS_REMAINING_ID = 61
  
# Custom Radio Settings
NV_ALT_RADIO_TRIM_ID = 63    

# The exact meaning of this field is Vendor Specific
NV_VENDOR_SETTINGS_ID = 64

#Range reserved for User-Defined NV Params
NV_USER_MIN_ID =  128
NV_USER_MAX_ID = 254
