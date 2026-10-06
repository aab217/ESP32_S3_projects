import socket
import csv
import struct

ADC_NUM_OF_CONV = 500
ADC_NUM_OF_CHAN = 1

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(0.5)
sock.bind(('0.0.0.0', 54321))

CSV_FILE = 'esp32_data.csv'

with open(CSV_FILE, 'w', newline='', encoding='utf-8') as f:
    writer = csv.writer(f, delimiter=';')

    header = ['timestamp']
    for i in range(ADC_NUM_OF_CHAN):
        header.append(f'Hall1_{i}')

    writer.writerow(header)

print("\nДля выхода нажми ctrl+c")

try:
    while True:
        try:
            data, addr = sock.recvfrom(4096)

            # timestamp uint64_t = 8 байт
            if len(data) < 8:
                continue

            timestamp = struct.unpack_from('<Q', data, 0)[0]

            offset = 8

            # ADC uint16_t = 2 байта на значение
            expected_size = 8 + ADC_NUM_OF_CONV * 2

            if len(data) < expected_size:
                print(
                    f"Ошибка: пакет слишком короткий: "
                    f"{len(data)} байт, ожидалось минимум {expected_size}"
                )
                continue

            hall1 = struct.unpack_from(
                f'<{ADC_NUM_OF_CONV}I',
                data,
                offset
            )

            with open(CSV_FILE, 'a', newline='', encoding='utf-8') as f:
                writer = csv.writer(f, delimiter=';')

                for value in hall1:
                    writer.writerow([timestamp, value])

            print(f"[{timestamp}] ADC[0]={hall1[0]}")

        except socket.timeout:
            pass

except KeyboardInterrupt:
    print("\nВыход")