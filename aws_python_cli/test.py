#!/usr/bin/env python

import pandas as pd
import awswrangler as wr

sql = '''
	SELECT
		*
	FROM
		"EmbebedCloud"."Axolote_Ivan_Table"
	WHERE
		time between ago(3h)
			and
		now()
	ORDER BY
		time DESC LIMIT 100
'''

df = wr.timestream.query(sql)
print( df[ (df['measure_name'] == 'press_hPa') | (df['measure_name'] == 'temp_C') ] )
