import time
import pandas as pd
import os

filename = "massive_server_logs.csv"
file_size = os.path.getsize(filename) / (1024**2)

print(f"📊 Analyzing {filename} ({file_size:.2f} MB)...")
start_time = time.time()

print("⏳ Loading into RAM (this might take a while)...")
df = pd.read_csv(filename)

avg_latency = df['latency_ms'].mean()

end_time = time.time()

print(f"✅ Average Latency: {avg_latency:.2f}")
print(f"⏱️ Python Time: {end_time - start_time:.2f} seconds")
print(f"💾 RAM Used: {df.memory_usage(deep=True).sum() / (1024**2):.2f} MB")
