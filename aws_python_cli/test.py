#!/usr/bin/env python

import pandas as pd
import awswrangler as wr

sql = '''
	SELECT * FROM "EmbebedCloud"."Axolote_Ivan_Table" WHERE time between ago(15m) and now() ORDER BY time DESC LIMIT 10 
'''

df = wr.timestream.query(sql)
print(df)
