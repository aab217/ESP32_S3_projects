import socket
import csv
import struct

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(0.5)  # Проверяем каждые 0.5 секунды
sock.bind(('0.0.0.0', 54321))

with open('esp32_data.csv', 'w', newline = '') as f:
    writer = csv.writer(f)
    writer.writerow(['var1', 'var2', 'var3'])

print("\nДля выхода нажми ctrl+c")

try:
    while True:
        try:
            data, addr = sock.recvfrom(1024)
            if len(data) >= 8:
                var1 = struct.unpack('<I', data[0:4])[0]
                var2 = struct.unpack('<H', data[4:6])[0]
                var3 = struct.unpack('<H', data[6:8])[0]

                print(f"Received: {var1} | {var2} | {var3}")

                with open('esp32_data.csv', 'a', newline = '') as f:
                    writer = csv.writer(f, delimiter=';')
                    writer.writerow([var1, var2, var3])
                    
        except socket.timeout:
            pass  
except KeyboardInterrupt:
    print("\nВыход")