# Generated by Django 2.1.3 on 2018-12-03 11:35

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('products', '0011_auto_20181202_2356'),
    ]

    operations = [
        migrations.AddField(
            model_name='products',
            name='is_costume',
            field=models.BooleanField(default=True),
        ),
    ]