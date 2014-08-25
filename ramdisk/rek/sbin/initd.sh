#!/sbin/busybox sh
PATH=/sbin/:/rek/sbin/:$PATH

/rek/sbin/busybox mount -o remount,rw /

echo "[START] remounting system" > /rek/logs/initdlog.txt
/rek/sbin/busybox mount -o remount,rw /system >> /rek/logs/initdlog.txt 

echo "[*] make init.d directory" >> /rek/logs/initdlog.txt
/rek/sbin/busybox mkdir -p /system/etc/init.d >> /rek/logs/initdlog.txt

echo "[*] correcting permissions of files in init.d directory" >> /rek/logs/initdlog.txt
/rek/sbin/busybox chmod 777 /system/etc/init.d/*


echo "[*] dnsmasq activation" >> /rek/logs/initdlog.txt
cp /rek/sbin/KaDNS /system/etc/init.d

echo "[*] UKSM activation" >> /rek/logs/initdlog.txt
if [ ! -e /system/etc/init.d/KaUKS ];
then
	/rek/sbin/busybox cp /rek/sbin/KaUKS /system/etc/init.d
fi

echo "[*] Starting init.d scripts" >> /rek/logs/initdlog.txt
/rek/sbin/busybox run-parts /system/etc/init.d

echo "[DONE] all done exiting" >> /rek/logs/initdlog.txt
/rek/sbin/busybox mount -o remount,ro /system >> /rek/logs/initdlog.txt
/rek/sbin/busybox mount -o remount,ro /
