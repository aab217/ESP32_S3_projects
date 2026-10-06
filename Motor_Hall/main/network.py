import socket
import csv
import struct

ADC_NUM_OF_CONV = 64  # Должно соответствовать значению в config.h

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(0.5)  # Проверяем каждые 0.5 секунды
sock.bind(('0.0.0.0', 54321))
CSV_FILE = 'esp32_data.csv'

with open(CSV_FILE, 'w', newline='', encoding='utf-8') as f:
    writer = csv.writer(f, delimiter=';')
    # Заголовок
    header = ['timestamp']
    for i in range(ADC_NUM_OF_CONV):
        header.append(f'Hall1_{i}')
    for i in range(ADC_NUM_OF_CONV):
        header.append(f'Hall2_{i}')
    writer.writerow(header)

print("\nДля выхода нажми ctrl+c")

try:
    while True:
        try:
            data, addr = sock.recvfrom(4096)

            if len(data) < 8:
                continue

            # Распаковка пакета
            offset = 0
            timestamp = struct.unpack_from('<Q', data, offset)[0]
            offset += 8

            hall1 = struct.unpack_from(f'<{ADC_NUM_OF_CONV}I', data, offset)
            offset += ADC_NUM_OF_CONV * 4
            hall2 = struct.unpack_from(f'<{ADC_NUM_OF_CONV}I', data, offset)

            # Формируем строку
            row = [timestamp] + list(hall1) + list(hall2)

            # Быстрая запись в CSV
            with open(CSV_FILE, 'a', newline='', encoding='utf-8') as f:
                writer = csv.writer(f, delimiter=';')
                writer.writerow(row)

            # Небольшой вывод в консоль (можно закомментировать для максимальной скорости)
            print(f"[{timestamp}] Hall1[0]={hall1[0]}, Hall2[0]={hall2[0]}")
            
        except socket.timeout:
            pass  
except KeyboardInterrupt:
    print("\nВыход")