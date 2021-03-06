Select Typ_alarmu_ID, [3], [4], [6], [9], [12], [17], [19], [21], [22], [24], [27], [29], [30], [33], [36], [38], [39]
from 
	(SELECT ar.Typ_alarmu_ID, Count(ar.Rekord_ID) as Ile, ar.Konfiguracja_ID
	FROM (select distinct Wyniki_alarm.Rekord_ID, Program_alarm.Typ_alarmu_ID, Program_alarm.Konfiguracja_ID
		  from Wyniki_alarm 
		  left join Program_alarm on Program_alarm.Program_alarm_ID = Wyniki_alarm.Program_alarm_ID
		  where Wyniki_alarm.Status>0 and (Program_alarm.Typ_alarmu_ID=0 or Program_alarm.Typ_alarmu_ID=3 or Program_alarm.Typ_alarmu_ID=5)
		  ) as ar
	 group by ar.Typ_alarmu_ID, ar.Konfiguracja_ID) as data
PIVOT(
min(Ile)
FOR Konfiguracja_ID IN([3], [4], [6], [9], [12], [17], [19], [21], [22], [24], [27], [29], [30], [33], [36], [38], [39])
) as pvt