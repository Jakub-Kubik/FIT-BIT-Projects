# Generated by Django 2.1.3 on 2018-12-02 16:38

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('products', '0002_auto_20181202_1550'),
    ]

    operations = [
        migrations.AlterField(
            model_name='products',
            name='manager',
            field=models.ForeignKey(db_column='manager', on_delete=django.db.models.deletion.DO_NOTHING, to=settings.AUTH_USER_MODEL),
        ),
    ]
