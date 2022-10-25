#!/usr/bin/env python

import numpy as np
import pandas as pd
import awswrangler as wr

# sql = '''
# 	SELECT
# 		*
# 	FROM
# 		"EmbebedCloud"."Axolote_Ivan_Table"
	
# '''

# df = wr.timestream.query(sql)

# df.to_csv('old.csv')

df = pd.read_csv('old.csv')

# temp_C has been logged as measure_value::varchar
# press_hPa has been logged as measure_value::bigint
# Both needs to be selected from column: measure_name

# First add the column for double
df['measure_value::double'] = np.NaN

# Now set the value for temp
df.loc[ df['measure_name'] == 'temp_C', 'measure_value::double'] = df[ df['measure_name'] == 'temp_C' ]['measure_value::varchar'].astype(np.double)

# Remove values
df.loc[ df['measure_name'] == 'temp_C', 'measure_value::varchar'] = np.NaN

# Same for presure
df.loc[ df['measure_name'] == 'press_hPa', 'measure_value::double'] = df[ df['measure_name'] == 'press_hPa' ]['measure_value::bigint'].astype(np.double)

# And the "time" column needs to be datetime
df['time'] = pd.to_datetime(df['time'])

# Remove the undesired columns
#df = df.drop('measure_value::varchar')
df = df.drop('measure_value::bigint', axis=1)

df.to_csv('new.csv')

rejected_records = wr.timestream.write(
    df=df,
    database="EmbebedCloud",
    table="Axolote_Table_Ivan",
    time_col="time",
    measure_col="measure_name",
    dimensions_cols=["mac_Id"],
)

print(f'{len(rejected_records)} records were rejected!')
assert len(rejected_records) == 0